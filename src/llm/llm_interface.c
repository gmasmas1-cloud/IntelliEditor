#include <stdio.h>
#include <stdlib.h>
#include "../../llama.cpp/include/llama.h"
#include <string.h>

/*
 * LLMEngine est notre "boîte" qui contient tout ce dont
 * on a besoin pour parler au modèle IA.
 * On la cache ici — les autres fichiers ne voient pas dedans.
 */
struct LLMEngine {
    struct llama_model*   model;  /* le modèle chargé en RAM   */
    struct llama_context* ctx;    /* l'espace de travail du LLM */
};

/*
 * llm_init() — charge le modèle et prépare le contexte.
 *
 * Paramètres :
 *   model_path : chemin vers le fichier .gguf
 *   n_threads  : combien de coeurs CPU utiliser (ex: 4)
 *   n_ctx      : taille de la "mémoire" du LLM (ex: 2048 tokens)
 *
 * Retourne : un pointeur vers LLMEngine, ou NULL si échec.
 */
struct LLMEngine* llm_init(const char* model_path,
                            int n_threads, int n_ctx) {
    /* Étape 1 : démarrer le moteur llama.cpp */
    llama_backend_init();

    /* Étape 2 : configurer le chargement du modèle */
    struct llama_model_params mparams = llama_model_default_params();
    /* llama_model_default_params() nous donne des valeurs raisonnables
     * par défaut — on n'a pas besoin de tout configurer manuellement */

    /* Étape 3 : charger le fichier .gguf depuis le disque */
    struct llama_model* model = llama_model_load_from_file(
                                    model_path, mparams);
    if (!model) {
        /* Si model est NULL, le fichier n'existe pas ou est corrompu */
        fprintf(stderr, "[LLM] Erreur : impossible de charger %s\n",
                model_path);
        return NULL;
    }
    printf("[LLM] Modèle chargé : %s\n", model_path);

    /* Étape 4 : créer le contexte (espace de travail) */
    struct llama_context_params cparams = llama_context_default_params();
    cparams.n_ctx     = n_ctx;      /* mémoire : 2048 tokens    */
    cparams.n_threads = n_threads;  /* 4 coeurs CPU             */

    struct llama_context* ctx = llama_init_from_model(model, cparams);
    if (!ctx) {
        fprintf(stderr, "[LLM] Erreur : impossible de créer le contexte\n");
        llama_model_free(model);
        return NULL;
    }

    /* Étape 5 : emballer model + ctx dans notre structure */
    struct LLMEngine* engine = malloc(sizeof(struct LLMEngine));
    if (!engine) {
        fprintf(stderr, "[LLM] Erreur : mémoire insuffisante\n");
        llama_free(ctx);
        llama_model_free(model);
        return NULL;
    }
    engine->model = model;
    engine->ctx   = ctx;

    printf("[LLM] Moteur prêt. Contexte : %d tokens, %d threads\n",
           n_ctx, n_threads);
    return engine;
}

/*
 * llm_free() — libère toute la mémoire utilisée par le moteur.
 *
 * En C, tout ce qu'on alloue avec malloc() doit être libéré.
 * Si on oublie, la mémoire est gaspillée jusqu'à la fermeture
 * du programme — c'est une "fuite mémoire".
 */
void llm_free(struct LLMEngine* engine) {
    if (!engine) return; /* sécurité : ne rien faire si NULL */
    llama_free(engine->ctx);        /* libère le contexte  */
    llama_model_free(engine->model);/* libère le modèle    */
    llama_backend_free();           /* arrête llama.cpp    */
    free(engine);                   /* libère notre struct */
    printf("[LLM] Moteur arrêté proprement.\n");
}



/*
 * llm_ask() — envoie un prompt au modèle et retourne la réponse.
 *
 * Paramètres :
 *   engine : le moteur initialisé par llm_init()
 *   prompt : la question en texte (ex: "Corrige cette phrase...")
 *
 * Retourne : la réponse en texte, allouée dynamiquement.
 *            L'appelant doit libérer avec free().
 *            Retourne NULL si erreur.
 */
char* llm_ask(struct LLMEngine* engine, const char* prompt) {
    if (!engine || !prompt) return NULL;

    /* --- ÉTAPE 1 : TOKENISER LE PROMPT --- */
    /* On demande combien de tokens maximum le contexte peut tenir */
    int n_ctx_max = llama_n_ctx(engine->ctx);

    /* On alloue un tableau pour stocker les tokens */
    llama_token* tokens = malloc(sizeof(llama_token) * n_ctx_max);
    if (!tokens) return NULL;

    /* On récupère le vocabulaire du modèle */
    const struct llama_vocab* vocab = llama_model_get_vocab(engine->model);

    /* On convertit le texte en tokens
     * llama_tokenize retourne le nombre de tokens produits */
    int n_tokens = llama_tokenize(vocab, prompt, strlen(prompt),
                                  tokens, n_ctx_max, true, false);
    if (n_tokens < 0) {
        fprintf(stderr, "[LLM] Erreur tokenisation\n");
        free(tokens);
        return NULL;
    }

    /* --- ÉTAPE 2 : ENVOYER LES TOKENS AU MODÈLE --- */
    /* Un "batch" c'est un paquet de tokens à traiter en une fois */
    struct llama_batch batch = llama_batch_get_one(tokens, n_tokens);

    if (llama_decode(engine->ctx, batch) != 0) {
        fprintf(stderr, "[LLM] Erreur decode\n");
        free(tokens);
        return NULL;
    }

    /* --- ÉTAPE 3 : GÉNÉRER LA RÉPONSE TOKEN PAR TOKEN --- */
    /* On prépare un buffer pour la réponse */
    size_t buf_size = 4096;
    char*  response = malloc(buf_size);
    if (!response) { free(tokens); return NULL; }
    response[0] = '\0';
    size_t pos = 0;

    /* Le sampler choisit le prochain token à chaque étape
     * "greedy" = toujours choisir le token le plus probable */
    struct llama_sampler* sampler = llama_sampler_chain_init(
        llama_sampler_chain_default_params()
    );
    llama_sampler_chain_add(sampler, llama_sampler_init_greedy());

    /* On génère jusqu'à 512 tokens maximum */
    for (int i = 0; i < 512; i++) {

        /* Choisir le prochain token */
        llama_token token_id = llama_sampler_sample(
                                   sampler, engine->ctx, -1);

        /* Si c'est un token de fin → le modèle a fini de répondre */
        if (llama_vocab_is_eog(vocab, token_id)) break;

        /* Convertir le token en texte */
        char piece[32];
        int n = llama_token_to_piece(vocab, token_id,
                                     piece, sizeof(piece), 0, false);

        if (n > 0) {
            /* Agrandir le buffer si nécessaire */
            if (pos + n + 1 >= buf_size) {
                buf_size *= 2;
                char* tmp = realloc(response, buf_size);
                if (!tmp) break;
                response = tmp;
            }
            /* Ajouter le morceau de texte à la réponse */
            memcpy(response + pos, piece, n);
            pos += n;
            response[pos] = '\0';
        }

        /* Envoyer ce token au modèle pour qu'il prépare le suivant */
        struct llama_batch next = llama_batch_get_one(&token_id, 1);
        if (llama_decode(engine->ctx, next) != 0) break;
    }

    llama_sampler_free(sampler);
    free(tokens);
    return response;
}

/* Nettoie la réponse du LLM : supprime le token <|im_end|> */
void llm_nettoyer_reponse(char* response) {
    if (!response) return;
    char* fin = strstr(response, "<|im_end|>");
    if (fin) *fin = '\0';
    /* Supprimer les espaces en fin */
    int len = strlen(response);
    while (len > 0 && (response[len-1] == ' ' ||
                        response[len-1] == '\n'))
        response[--len] = '\0';
}
