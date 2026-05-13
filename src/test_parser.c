#include "rules.h"
#include <stdio.h>

int main() {
    // Parser le fichier de règles
    RuleSet *rule_set = parse_rules("data/rules/memoire_licence.json");

    if (!rule_set) {
        printf(" Échec du parsing.\n");
        return 1;
    }

    // Afficher les règles chargées
    printf("✅ Parsing réussi ! %d règles chargées.\n\n", rule_set->rule_count);

    for (int i = 0; i < rule_set->rule_count; i++) {
        Rule rule = rule_set->rules[i];
        printf("Règle %d:\n", i + 1);
        printf("  - ID: %s\n", rule.id);
        printf("  - Description: %s\n", rule.description);
        printf("  - Type: %d\n", rule.check_type);
        printf("  - Paramètre: %s\n\n", rule.parameter ? rule.parameter : "Aucun");
    }

    // Libérer la mémoire
    free_rules(rule_set);
    return 0;
}