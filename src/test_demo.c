// src/test_demo.c
#include <stdio.h>
#include <stdlib.h>
#include "rule_parser.h"
#include "rule_engine.h"

int main() {
    // 1. Charger un template de règles (ex: mémoire de licence)
    char *json_string = read_file("data/rule_templates/memoire_licence.json");
    if (json_string == NULL) {
        printf("Le fichier de regles introuvable.\n");
        return 1;
    }

    RuleSet *ruleset = parse_rules(json_string);
    free(json_string);

    if (ruleset == NULL) {
        printf("Impossible de parser le JSON.\n");
        return 1;
    }

    // 2. Charger le texte de démo
    char *text = read_file("data/demo_text.txt");
    if (text == NULL) {
        printf("Le fichier demo_text.txt est introuvable.\n");
        free_rules(ruleset);
        return 1;
    }

    // 3. Vérifier les règles
    RuleReport *report = check_all_rules(ruleset, text);

    // 4. Afficher le rapport (simule l'UI)
    printf("=== Rapport de conformiter pour %s ===\n", ruleset->document_type);
    printf("Total: %d regles | tres bien %d | Alerte %d | supprimer %d\n\n",
        report->count, report->ok_count, report->warning_count, report->error_count);

    for (int i = 0; i < report->count; i++) {
        RuleResult *result = &report->results[i];
        const char *status_icon = "";
        const char *status_color = "";
        switch (result->status) {
            case RULE_OK: status_icon = "okay"; status_color = ""; break;
            case RULE_WARNING: status_icon = "alerte"; status_color = ""; break;
            case RULE_ERROR: status_icon = "supprimer"; status_color = ""; break;
            case RULE_PENDING: status_icon = "en cours de recherche"; status_color = ""; break;
        }
        printf("%s [%s] %s: %s\n", status_icon, result->rule_id, ruleset->rules[i].description, result->message);
    }

    // 5. Libérer la mémoire
    free(text);
    free_rule_report(report);
    free_rules(ruleset);

    printf("\n Demo terminer ! Pret pour l'integration avec l'UI demain.\n");
    return 0;
}