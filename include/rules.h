#ifndef RULES_H
#define RULES_H

// Types de sévérité pour les règles
typedef enum {
    RULE_ERROR,    // Erreur bloquante
    RULE_WARNING,  // Avertissement
    RULE_INFO      // Information
} RuleSeverity;

// Types de vérifications possibles
typedef enum {
    CHECK_SECTION_EXISTS,    // Vérifie si une section existe (ex: "Introduction")
    CHECK_WORD_COUNT_MIN,    // Vérifie le nombre minimum de mots
    CHECK_REGEX_FORBIDDEN    // Vérifie une regex interdite (ex: "je")
} CheckType;

// Structure pour une règle
typedef struct {
    char *id;               // Ex: "R001"
    char *category;         // Ex: "structure"
    RuleSeverity severity;  // Ex: RULE_ERROR
    char *description;      // Ex: "Le document doit contenir une Introduction"
    CheckType check_type;   // Ex: CHECK_SECTION_EXISTS
    char *parameter;        // Ex: "Introduction" (pour section_exists)
} Rule;

// Structure pour un ensemble de règles
typedef struct {
    Rule *rules;           // Tableau de règles
    int rule_count;        // Nombre de règles
} RuleSet;

// Fonction pour parser un fichier JSON de règles
RuleSet* parse_rules(const char *filepath);

// Fonction pour libérer la mémoire
void free_rules(RuleSet *rule_set);

#endif // RULES_H