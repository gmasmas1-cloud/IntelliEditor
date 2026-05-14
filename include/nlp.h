#ifndef NLP_H
#define NLP_H

/*
 * nlp.h — Interface publique du module NLP (DEV-C)
 *
 * Ce fichier est le seul que les autres modules doivent inclure.
 * Il expose les types et fonctions nécessaires pour :
 *   - DEV-B : afficher les erreurs dans Scintilla
 *   - DEV-D : vérifier le nombre de mots dans une section
 */

/* --- Types d'erreurs --- */
typedef enum {
    NLP_ORTHO,        /* faute d'orthographe  */
    NLP_GRAMMAIRE,    /* faute de grammaire   */
    NLP_PONCTUATION   /* erreur de ponctuation */
} NLPErreurType;

/* --- Une erreur trouvée dans le texte --- */
typedef struct {
    NLPErreurType type;
    int           position;   /* offset depuis le début du texte */
    char*         original;   /* mot original                    */
    char*         suggestion; /* correction proposée             */
} NLPErreur;

/* --- Résultat complet d'une analyse --- */
typedef struct {
    NLPErreur* erreurs;
    int        count;
} NLPResult;

/* --- Moteur Hunspell (opaque) --- */
typedef struct HunspellEngine HunspellEngine;

/* --- API Hunspell --- */
HunspellEngine* hunspell_init(void);
void            hunspell_free(HunspellEngine* hs);

/* --- API NLP --- */
NLPResult nlp_analyser(const char* texte, HunspellEngine* hs);
void      nlp_result_afficher(const NLPResult* result);
void      nlp_result_free(NLPResult* result);

/* --- API Tokenizer --- */
/* Utilisée par DEV-D pour compter les mots dans une section */
int tokenizer_compter_mots(const char* texte);

#endif /* NLP_H */
