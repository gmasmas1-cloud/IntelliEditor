#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hunspell/hunspell.h>

/*
 * Hunspell est une bibliothèque externe comme llama.cpp.
 * On l'utilise via son header hunspell.h.
 *
 * Hunhandle est le "moteur" de Hunspell — comme LLMEngine
 * pour llama.cpp. C'est une structure opaque définie par Hunspell.
 */

/* Notre structure qui contient le moteur Hunspell */
struct HunspellEngine {
    Hunhandle* handle;  /* pointeur vers le moteur Hunspell */
};

/*
 * hunspell_init() — charge le dictionnaire français.
 *
 * Retourne : un pointeur vers HunspellEngine, ou NULL si échec.
 */
struct HunspellEngine* hunspell_init(void) {
    struct HunspellEngine* hs = malloc(sizeof(struct HunspellEngine));
    if (!hs) return NULL;

    /* Charger le dictionnaire français
     * On a besoin des deux fichiers : .aff et .dic */
    hs->handle = Hunspell_create(
        "/usr/share/hunspell/fr_FR.aff",
        "/usr/share/hunspell/fr_FR.dic"
    );

    if (!hs->handle) {
        fprintf(stderr, "[Hunspell] Erreur : dictionnaire non trouvé\n");
        free(hs);
        return NULL;
    }

    printf("[Hunspell] Dictionnaire français chargé.\n");
    return hs;
}

/*
 * hunspell_verifier() — vérifie si un mot est correct.
 *
 * Retourne : 1 si correct, 0 si incorrect, -1 si erreur.
 *
 * Exemple :
 *   hunspell_verifier(hs, "bonjour") → 1
 *   hunspell_verifier(hs, "bnojour") → 0
 */
int hunspell_verifier(struct HunspellEngine* hs, const char* mot) {
    if (!hs || !mot) return -1;

    /* Hunspell_spell retourne 0 si le mot est INCORRECT
     * et non-zéro si le mot est CORRECT
     * On normalise : on retourne 1 si correct, 0 si incorrect */
    return Hunspell_spell(hs->handle, mot) ? 1 : 0;
}

/*
 * hunspell_suggestions() — retourne des corrections possibles.
 *
 * Si "bnojour" est incorrect, Hunspell propose ["bonjour", "bonsoir"...]
 *
 * Retourne : tableau de suggestions, nb_sugg contient le nombre.
 *            L'appelant doit libérer avec hunspell_liberer_suggestions().
 */
char** hunspell_suggestions(struct HunspellEngine* hs,
                             const char* mot, int* nb_sugg) {
    if (!hs || !mot || !nb_sugg) return NULL;

    char** liste = NULL;
    *nb_sugg = Hunspell_suggest(hs->handle, &liste, mot);

    if (*nb_sugg <= 0) {
        *nb_sugg = 0;
        return NULL;
    }
    return liste;
}

/*
 * hunspell_liberer_suggestions() — libère la liste de suggestions.
 */
void hunspell_liberer_suggestions(struct HunspellEngine* hs,
                                   char** liste, int nb_sugg) {
    if (!hs || !liste) return;
    Hunspell_free_list(hs->handle, &liste, nb_sugg);
}

/*
 * hunspell_free() — libère toutes les ressources.
 */
void hunspell_free(struct HunspellEngine* hs) {
    if (!hs) return;
    Hunspell_destroy(hs->handle);
    free(hs);
    printf("[Hunspell] Ressources libérées.\n");
}
