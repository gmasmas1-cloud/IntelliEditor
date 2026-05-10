#include <stdio.h>
#include <stdlib.h>
#include "../../llama.cpp/include/llama.h"

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
