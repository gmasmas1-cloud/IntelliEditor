// src/test_rule_checker.c
#include <stdio.h>
#include <stdlib.h>
#include "rule_parser.h"
#include "rule_checker.h"
#include "rule_engine.h"

int main() {
    // 1. Charger les règles depuis le fichier JSON
    char *json_string = read_file("data/test_rules.json");
    if (json_string == NULL) {
        printf("Fichier data/test_rules.json introuvable.\n");
        return 1;
    }

    RuleSet *ruleset = parse_rules(json_string);
    free(json_string);

    if (ruleset == NULL) {
        printf("Impossible de parser le JSON.\n");
        return 1;
    }

    // 2. Texte de test (simule un document)
    const char *text =
        "# Resumer\n"
        "Ce document est un test.\n\n"
        "# Introduction\n"
        "Voici une introduction avec assez de mots pour passer le test. "
        "Ce texte contient suffisamment de mots pour valider la regle.\n\n"
        "# Conclusion\n"
        "Ceci est la conclusion.";

    // 3. Vérifier toutes les règles
    RuleReport *report = check_all_rules(ruleset, text);

    // 4. Afficher le rapport
    printf("=== Rapport de conformiter ===\n");
    printf("Total: %d regles | OK: %d | Avertissements: %d | Erreurs: %d\n",
        report->count, report->ok_count, report->warning_count, report->error_count);

    for (int i = 0; i < report->count; i++) {
        RuleResult *result = &report->results[i];
        const char *status_str = "";
        switch (result->status) {
            case RULE_OK: status_str = "OK"; break;
            case RULE_WARNING: status_str = "WARNING"; break;
            case RULE_ERROR: status_str = "ERROR"; break;
            case RULE_PENDING: status_str = "PENDING"; break;
        }
        printf("\n Regle %s: %s\n", result->rule_id, status_str);
        printf("   %s\n", result->message);
    }

    // 5. Libérer la mémoire
    free_rule_report(report);
    free_rules(ruleset);

    printf("\n Le moteur de regles est fonctionnel !\n");
    return 0;
}