#include "rule_checker.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Vérificateur : Section existe ---
RuleResult check_section_exists(const char *text, const char *section_name) {
    RuleResult result = {0};
    result.rule_id = NULL;
    result.status = RULE_ERROR;
    result.message = NULL;

    // Cherche le nom de la section dans le texte (simplifié)
    // On cherche un titre de section comme "# Introduction" ou "== Introduction"
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\n# %s\n", section_name);
    if (strstr(text, pattern) != NULL) {
        result.status = RULE_OK;
        result.message = strdup("Section trouvée");
    } else {
        char msg[256];
        snprintf(msg, sizeof(msg), "Section '%s' manquante", section_name);
        result.message = strdup(msg);
    }
    return result;
}

// --- Compteur de mots ---
int count_words(const char *text) {
    int count = 0;
    int in_word = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        if (isspace(text[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            count++;
        }
    }
    return count;
}

// --- Vérificateur : Nombre de mots minimum ---
RuleResult check_word_count_min(const char *text, int min_words) {
    RuleResult result = {0};
    int word_count = count_words(text);

    if (word_count >= min_words) {
        result.status = RULE_OK;
        result.message = strdup("Nombre de mots suffisant");
    } else {
        result.status = RULE_WARNING;
        char msg[256];
        snprintf(msg, sizeof(msg), "Nombre de mots insuffisant (%d/%d)", word_count, min_words);
        result.message = strdup(msg);
    }
    return result;
}

// --- Vérificateur : Nombre de mots maximum ---
RuleResult check_word_count_max(const char *text, int max_words) {
    RuleResult result = {0};
    int word_count = count_words(text);

    if (word_count <= max_words) {
        result.status = RULE_OK;
        result.message = strdup("Nombre de mots acceptable");
    } else {
        result.status = RULE_WARNING;
        char msg[256];
        snprintf(msg, sizeof(msg), "Nombre de mots trop élevé (%d/%d)", word_count, max_words);
        result.message = strdup(msg);
    }
    return result;
}

// --- Vérificateur : Ordre des sections ---
RuleResult check_section_order(const char *text, char **sections, int section_count) {
    RuleResult result = {0};
    result.status = RULE_OK;
    result.message = strdup("Ordre des sections correct");

    // Trouver les positions de chaque section
    int *positions = (int*)malloc(section_count * sizeof(int));
    for (int i = 0; i < section_count; i++) {
        positions[i] = -1; // -1 = section non trouvée
        char pattern[256];
        snprintf(pattern, sizeof(pattern), "\n# %s\n", sections[i]);
        char *pos = strstr(text, pattern);
        if (pos != NULL) {
            positions[i] = pos - text; // Position dans le texte
        }
    }

    // Vérifier l'ordre
    for (int i = 0; i < section_count - 1; i++) {
        if (positions[i] == -1 || positions[i+1] == -1) {
            result.status = RULE_ERROR;
            free(result.message);
            result.message = strdup("Une ou plusieurs sections manquantes");
            break;
        }
        if (positions[i] > positions[i+1]) {
            result.status = RULE_ERROR;
            free(result.message);
            char msg[256];
            snprintf(msg, sizeof(msg), "Section '%s' doit venir avant '%s'", sections[i], sections[i+1]);
            result.message = strdup(msg);
            break;
        }
    }

    free(positions);
    return result;
}