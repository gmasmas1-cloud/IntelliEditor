#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Types d'erreurs */
typedef enum {
    NLP_ORTHO,       /* faute d'orthographe  */
    NLP_GRAMMAIRE,   /* faute de grammaire   */
    NLP_PONCTUATION  /* erreur de ponctuation */
} NLPErreurType;

/* Une erreur trouvée dans le texte */
typedef struct {
    NLPErreurType type;
    int           position;   /* offset dans le texte */
    char*         original;   /* mot original         */
    char*         suggestion; /* correction proposée  */
} NLPErreur;

/* Résultat complet de l'analyse */
typedef struct {
    NLPErreur* erreurs;
    int        count;
} NLPResult;

/* Déclarations des modules qu'on utilise */
struct HunspellEngine;
struct HunspellEngine* hunspell_init(void);
int    hunspell_verifier(struct HunspellEngine* hs, const char* mot);
char** hunspell_suggestions(struct HunspellEngine* hs,
                             const char* mot, int* nb_sugg);
void   hunspell_liberer_suggestions(struct HunspellEngine* hs,
                                     char** liste, int nb_sugg);
void   hunspell_free(struct HunspellEngine* hs);

typedef struct { char** tokens; int count; } TokenList;
TokenList tokenizer_mots(const char* texte);
void      tokenizer_free(TokenList* liste);

/*
 * nlp_analyser() — analyse un texte et retourne toutes les erreurs.
 *
 * C'est la fonction principale que DEV-B appellera depuis l'interface.
 * Elle coordonne Hunspell et le tokenizer.
 */
NLPResult nlp_analyser(const char* texte,
                        struct HunspellEngine* hs) {
    NLPResult result = {NULL, 0};
    if (!texte || !hs) return result;

    /* Allouer le tableau d'erreurs */
    int capacite = 32;
    result.erreurs = malloc(sizeof(NLPErreur) * capacite);
    if (!result.erreurs) return result;

    /* Découper le texte en mots */
    TokenList mots = tokenizer_mots(texte);

    /* Vérifier chaque mot avec Hunspell */
    int offset = 0;
    for (int i = 0; i < mots.count; i++) {
        const char* mot = mots.tokens[i];

        /* Trouver la position du mot dans le texte original */
        const char* pos = strstr(texte + offset, mot);
        int position = pos ? (int)(pos - texte) : offset;
        offset = position + strlen(mot);

        /* Vérifier l'orthographe */
        if (hunspell_verifier(hs, mot) == 0) {
            /* Mot incorrect — chercher une suggestion */
            int nb_sugg = 0;
            char** sugg = hunspell_suggestions(hs, mot, &nb_sugg);

            /* Agrandir si nécessaire */
            if (result.count >= capacite) {
                capacite *= 2;
                NLPErreur* tmp = realloc(result.erreurs,
                                          sizeof(NLPErreur) * capacite);
                if (!tmp) break;
                result.erreurs = tmp;
            }

            /* Ajouter l'erreur */
            NLPErreur* e = &result.erreurs[result.count++];
            e->type       = NLP_ORTHO;
            e->position   = position;
            e->original   = strdup(mot);
            e->suggestion = (nb_sugg > 0) ? strdup(sugg[0]) : NULL;

            if (sugg) hunspell_liberer_suggestions(hs, sugg, nb_sugg);
        }
    }

    tokenizer_free(&mots);
    return result;
}

/*
 * nlp_result_afficher() — affiche les erreurs (pour debug).
 */
void nlp_result_afficher(const NLPResult* result) {
    if (!result) return;
    printf("[NLP] %d erreur(s) trouvée(s) :\n", result->count);
    for (int i = 0; i < result->count; i++) {
        const NLPErreur* e = &result->erreurs[i];
        printf("  pos=%d '%s' → '%s'\n",
               e->position,
               e->original   ? e->original   : "?",
               e->suggestion ? e->suggestion : "?");
    }
}

/*
 * nlp_result_free() — libère un NLPResult.
 */
void nlp_result_free(NLPResult* result) {
    if (!result || !result->erreurs) return;
    for (int i = 0; i < result->count; i++) {
        free(result->erreurs[i].original);
        free(result->erreurs[i].suggestion);
    }
    free(result->erreurs);
    result->erreurs = NULL;
    result->count   = 0;
}
