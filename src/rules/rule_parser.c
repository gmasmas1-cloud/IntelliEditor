#include "cJSON.h"      // Bibliothèque pour parser le JSON
#include "rules.h"      // Nos structures
#include <stdio.h>      // Pour fopen, fread, etc.
#include <stdlib.h>     // Pour malloc, free, strdup
#include <string.h>     // Pour strcmp

// Fonction pour convertir une chaîne en RuleSeverity
RuleSeverity string_to_severity(const char *severity_str) {
    if (strcmp(severity_str, "error") == 0) return RULE_ERROR;
    if (strcmp(severity_str, "warning") == 0) return RULE_WARNING;
    return RULE_INFO;  // Par défaut
}

// Fonction pour convertir une chaîne en CheckType
CheckType string_to_check_type(const char *check_type_str) {
    if (strcmp(check_type_str, "section_exists") == 0) return CHECK_SECTION_EXISTS;
    if (strcmp(check_type_str, "word_count_min") == 0) return CHECK_WORD_COUNT_MIN;
    if (strcmp(check_type_str, "regex_forbidden") == 0) return CHECK_REGEX_FORBIDDEN;
    return CHECK_SECTION_EXISTS;  // Par défaut
}

// Fonction principale : Parse un fichier JSON de règles
RuleSet* parse_rules(const char *filepath) {
    // 1. Ouvrir le fichier
    FILE *file = fopen(filepath, "r");
    if (!file) {
        perror("Erreur: Impossible d'ouvrir le fichier");
        return NULL;
    }

    // 2. Lire le contenu du fichier
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(file_size + 1);
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';  // Ajouter un terminateur
    fclose(file);

    // 3. Parser le JSON avec cJSON
    cJSON *json = cJSON_Parse(buffer);
    free(buffer);  // Libérer le buffer

    if (!json) {
        fprintf(stderr, "Erreur: JSON invalide.\n");
        return NULL;
    }

    // 4. Extraire le tableau "rules"
    cJSON *rules_array = cJSON_GetObjectItemCaseSensitive(json, "rules");
    if (!cJSON_IsArray(rules_array)) {
        fprintf(stderr, "Erreur: 'rules' doit être un tableau.\n");
        cJSON_Delete(json);
        return NULL;
    }

    // 5. Compter le nombre de règles
    int rule_count = cJSON_GetArraySize(rules_array);
    Rule *rules = (Rule*)malloc(rule_count * sizeof(Rule));

    // 6. Remplir le tableau de règles
    for (int i = 0; i < rule_count; i++) {
        cJSON *rule_json = cJSON_GetArrayItem(rules_array, i);

        Rule rule;
        // Extraire les champs du JSON
        rule.id = strdup(cJSON_GetObjectItemCaseSensitive(rule_json, "id")->valuestring);
        rule.category = strdup(cJSON_GetObjectItemCaseSensitive(rule_json, "category")->valuestring);
        rule.description = strdup(cJSON_GetObjectItemCaseSensitive(rule_json, "description")->valuestring);

        // Convertir les chaînes en enum
        cJSON *severity_json = cJSON_GetObjectItemCaseSensitive(rule_json, "severity");
        rule.severity = string_to_severity(severity_json->valuestring);

        cJSON *check_type_json = cJSON_GetObjectItemCaseSensitive(rule_json, "check_type");
        rule.check_type = string_to_check_type(check_type_json->valuestring);

        // Extraire le paramètre (ex: "Introduction" pour section_exists)
        cJSON *parameter_json = cJSON_GetObjectItemCaseSensitive(rule_json, "parameter");
        if (cJSON_IsString(parameter_json)) {
            rule.parameter = strdup(parameter_json->valuestring);
        } else {
            rule.parameter = NULL;  // Pas de paramètre
        }

        rules[i] = rule;
    }

    // 7. Créer le RuleSet
    RuleSet *rule_set = (RuleSet*)malloc(sizeof(RuleSet));
    rule_set->rules = rules;
    rule_set->rule_count = rule_count;

    // 8. Libérer le JSON
    cJSON_Delete(json);

    return rule_set;
}

// Fonction pour libérer la mémoire
void free_rules(RuleSet *rule_set) {
    if (!rule_set) return;

    for (int i = 0; i < rule_set->rule_count; i++) {
        free(rule_set->rules[i].id);
        free(rule_set->rules[i].category);
        free(rule_set->rules[i].description);
        free(rule_set->rules[i].parameter);
    }
    free(rule_set->rules);
    free(rule_set);
}