#include <stdio.h>
#include <stdlib.h>
#include "rule_parser.h"

int main() {
    // 1. Lire le fichier JSON
    char *json_string = read_file("data/test_rules.json");
    if (json_string == NULL) {
        printf("Le Fichier data/test_rules.json est introuvable.\n");
        return 1;
    }

    // 2. Parser les règles
    RuleSet *ruleset = parse_rules(json_string);
    free(json_string);  // Libérer la mémoire du fichier

    if (ruleset == NULL) {
        printf("Impossible de parser le JSON.\n");
        return 1;
    }

    // 3. Afficher les règles
    print_rules(ruleset);

    // 4. Libérer la mémoire
    free_rules(ruleset);

    printf("\n Parseur de règles est bien fonctionnel !\n");
    return 0;
}