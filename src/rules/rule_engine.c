// src/rules/rule_engine.c
#include "rule_parser.h"
#include "rule_checker.h"
#include "rule_engine.h"
#include <stdlib.h>
#include <string.h>

// Vérifie toutes les règles et génère un rapport
RuleReport* check_all_rules(RuleSet *ruleset, const char *text) {
    RuleReport *report = (RuleReport*)malloc(sizeof(RuleReport));
    report->results = (RuleResult*)malloc(ruleset->rule_count * sizeof(RuleResult));
    report->count = ruleset->rule_count;
    report->ok_count = 0;
    report->warning_count = 0;
    report->error_count = 0;

    for (int i = 0; i < ruleset->rule_count; i++) {
        Rule *rule = &ruleset->rules[i];
        RuleResult result = {0};
        result.rule_id = strdup(rule->id); // Copie l'ID de la règle

        // Appelle le vérificateur correspondant au type de règle
        if (strcmp(rule->check_type, "section_exists") == 0) {
            result = check_section_exists(text, rule->parameter);
            free(result.rule_id); // On garde celui de la règle
            result.rule_id = strdup(rule->id);
        }
        else if (strcmp(rule->check_type, "word_count_min") == 0) {
            int min_words = atoi(rule->parameter);
            result = check_word_count_min(text, min_words);
            free(result.rule_id);
            result.rule_id = strdup(rule->id);
        }
        else if (strcmp(rule->check_type, "word_count_max") == 0) {
            int max_words = atoi(rule->parameter);
            result = check_word_count_max(text, max_words);
            free(result.rule_id);
            result.rule_id = strdup(rule->id);
        }
        else if (strcmp(rule->check_type, "section_order") == 0) {
            // Pour section_order, le paramètre est un JSON array comme ["Résumé", "Introduction"]
            // On simplifie ici en utilisant une liste fixe pour l'exemple
            char *sections[] = {"Résumé", "Introduction", "Conclusion"};
            result = check_section_order(text, sections, 3);
            free(result.rule_id);
            result.rule_id = strdup(rule->id);
        }
        else if (strcmp(rule->check_type, "regex_forbidden") == 0) {
            int case_insensitive = 0; // Par défaut
            result = check_regex_forbidden(text, rule->parameter, case_insensitive);
            free(result.rule_id);
            result.rule_id = strdup(rule->id);
        }
        else if (strcmp(rule->check_type, "heading_format") == 0) {
            // Parsing du paramètre JSON (ex: {"level": 1, "case": "uppercase"})
            // Pour simplifier, on utilise des valeurs fixes ici
            int level = 1;
            const char *case_type = "uppercase";
            result = check_heading_format(text, level, case_type);
            free(result.rule_id);
            result.rule_id = strdup(rule->id);
        } else {
            // Type de règle non implémenté
            result.status = RULE_PENDING;
            result.message = strdup("Vérificateur non implémenté");
            result.rule_id = strdup(rule->id);
        }

        report->results[i] = result;

        // Mettre à jour les compteurs
        if (result.status == RULE_OK) report->ok_count++;
        else if (result.status == RULE_WARNING) report->warning_count++;
        else if (result.status == RULE_ERROR) report->error_count++;
    }

    return report;
}

// Libère la mémoire du rapport
void free_rule_report(RuleReport *report) {
    if (report == NULL) return;
    for (int i = 0; i < report->count; i++) {
        free(report->results[i].rule_id);
        free(report->results[i].message);
    }
    free(report->results);
    free(report);
}