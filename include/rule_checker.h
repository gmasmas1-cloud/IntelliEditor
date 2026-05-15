#ifndef RULE_CHECKER_H
#define RULE_CHECKER_H

#include "rule_parser.h"

// Statuts possibles pour une règle
typedef enum {
    RULE_OK,        // Conforme
    RULE_WARNING,   // Avertissement
    RULE_ERROR,     // Non conforme
    RULE_PENDING    // En attente (pour les vérifications LLM plus tard)
} RuleStatus;

// Résultat d'une vérification de règle
typedef struct {
    char *rule_id;      // ID de la règle (ex: "R001")
    RuleStatus status;  // Statut (OK, WARNING, ERROR...)
    char *message;      // Message explicatif (ex: "Section manquante")
} RuleResult;

// Rapport complet de toutes les règles
typedef struct {
    RuleResult *results;   // Tableau de résultats
    int count;             // Nombre total de règles
    int ok_count;          // Nombre de règles OK
    int warning_count;    // Nombre d'avertissements
    int error_count;      // Nombre d'erreurs
} RuleReport;

// Fonctions
RuleResult check_section_exists(const char *text, const char *section_name);
RuleResult check_word_count_min(const char *text, int min_words);
RuleResult check_word_count_max(const char *text, int max_words);
RuleResult check_section_order(const char *text, char **sections, int section_count);
// ajout
RuleResult check_regex_forbidden(const char *text, const char *pattern, int case_insensitive);
RuleResult check_heading_format(const char *text, int level, const char *case_type);

RuleReport* check_all_rules(RuleSet *ruleset, const char *text);
void free_rule_report(RuleReport *report);

#endif