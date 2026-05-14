#include <stdio.h>
#include <stdlib.h>

/* Types nécessaires — dans le bon ordre */
typedef enum {
    NLP_ORTHO,
    NLP_GRAMMAIRE,
    NLP_PONCTUATION
} NLPErreurType;

typedef struct {
    NLPErreurType type;
    int           position;
    char*         original;
    char*         suggestion;
} NLPErreur;

typedef struct {
    NLPErreur* erreurs;
    int        count;
} NLPResult;

/* Déclarations */
struct HunspellEngine;
struct HunspellEngine* hunspell_init(void);
void      hunspell_free(struct HunspellEngine* hs);
NLPResult nlp_analyser(const char* texte, struct HunspellEngine* hs);
void      nlp_result_afficher(const NLPResult* result);
void      nlp_result_free(NLPResult* result);

int main() {
    printf("--- Test DEV-C : NLP complet ---\n\n");

    struct HunspellEngine* hs = hunspell_init();
    if (!hs) return 1;

    const char* texte = "Les enfant mangent des pomme. Je vais au école.";
    printf("Texte : %s\n\n", texte);

    NLPResult result = nlp_analyser(texte, hs);
    nlp_result_afficher(&result);
    nlp_result_free(&result);

    hunspell_free(hs);
    return 0;
}
