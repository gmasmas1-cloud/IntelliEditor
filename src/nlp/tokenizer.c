#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 * TokenList — une liste de mots extraits d'un texte.
 * tokens : tableau de chaînes
 * count  : nombre de mots
 */
typedef struct {
    char** tokens;
    int    count;
} TokenList;
void tokenizer_free(TokenList* liste);

/*
 * tokenizer_mots() — découpe un texte en mots.
 *
 * En français, l'apostrophe et le tiret font partie des mots :
 * "aujourd'hui" = 1 mot, "porte-monnaie" = 1 mot.
 *
 * Séparateurs : espace, virgule, point, !, ?, ;, :, (, ), "
 */
TokenList tokenizer_mots(const char* texte) {
    TokenList result = {NULL, 0};
    if (!texte) return result;

    /* Allouer le tableau de tokens */
    int capacite = 64;
    result.tokens = malloc(sizeof(char*) * capacite);
    if (!result.tokens) return result;

    const char* p = texte;
    while (*p) {
        /* Sauter les séparateurs */
        while (*p && (isspace((unsigned char)*p) ||
               *p == ',' || *p == '.' || *p == '!' ||
               *p == '?' || *p == ';' || *p == ':' ||
               *p == '(' || *p == ')' || *p == '"'))
            p++;

        if (!*p) break;

        /* Trouver la fin du mot */
        const char* debut = p;
        while (*p && !isspace((unsigned char)*p) &&
               *p != ',' && *p != '.' && *p != '!' &&
               *p != '?' && *p != ';' && *p != ':' &&
               *p != '(' && *p != ')' && *p != '"')
            p++;

        int longueur = (int)(p - debut);
        if (longueur == 0) continue;

        /* Agrandir si nécessaire */
        if (result.count >= capacite) {
            capacite *= 2;
            char** tmp = realloc(result.tokens,
                                  sizeof(char*) * capacite);
            if (!tmp) break;
            result.tokens = tmp;
        }

        /* Copier le mot */
        result.tokens[result.count] = malloc(longueur + 1);
        if (!result.tokens[result.count]) break;
        memcpy(result.tokens[result.count], debut, longueur);
        result.tokens[result.count][longueur] = '\0';
        result.count++;
    }

    return result;
}

/*
 * tokenizer_compter_mots() — compte le nombre de mots dans un texte.
 * Utilisé par DEV-D pour les règles word_count_min/max.
 */
int tokenizer_compter_mots(const char* texte) {
    if (!texte) return 0;
    TokenList liste = tokenizer_mots(texte);
    int count = liste.count;
    tokenizer_free(&liste);
    return count;
}

/*
 * tokenizer_free() — libère une TokenList.
 */
void tokenizer_free(TokenList* liste) {
    if (!liste) return;
    for (int i = 0; i < liste->count; i++)
        free(liste->tokens[i]);
    free(liste->tokens);
    liste->tokens = NULL;
    liste->count  = 0;
}
