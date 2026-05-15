#ifndef RULE_PARSER_H
#define RULE_PARSER_H

// Une règle individuelle
typedef struct {
    char *id;          // Ex: "R001"
    char *category;    // Ex: "structure"
    char *severity;    // Ex: "error"
    char *description; // Ex: "Le document doit..."
    char *check_type;   // Ex: "section_exists"
    char *parameter;    // Ex: "Introduction"
} Rule;

// Un ensemble de règles
typedef struct {
    char *document_type; // Ex: "Mémoire de Licence"
    char *version;       // Ex: "1.0"
    Rule *rules;         // Tableau de règles
    int rule_count;      // Nombre de règles
} RuleSet;

// Fonctions
char* read_file(const char *path);
RuleSet* parse_rules(const char *json_string);
void free_rules(RuleSet *ruleset);
void print_rules(RuleSet *ruleset);

#endif