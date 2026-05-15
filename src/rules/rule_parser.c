#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"
#include "rule_parser.h"  // Chemin correct grâce à CMake

char* read_file(const char *path) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char*)malloc(length + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0';
    fclose(file);
    return buffer;
}

RuleSet* parse_rules(const char *json_string) {
    cJSON *json = cJSON_Parse(json_string);
    if (json == NULL) return NULL;

    RuleSet *ruleset = (RuleSet*)malloc(sizeof(RuleSet));
    if (ruleset == NULL) {
        cJSON_Delete(json);
        return NULL;
    }

    ruleset->document_type = NULL;
    ruleset->version = NULL;
    ruleset->rules = NULL;
    ruleset->rule_count = 0;

    // Métadonnées
    cJSON *meta = cJSON_GetObjectItem(json, "meta");
    if (meta != NULL) {
        cJSON *doc_type = cJSON_GetObjectItem(meta, "document_type");
        if (doc_type != NULL && cJSON_IsString(doc_type)) {
            ruleset->document_type = strdup(doc_type->valuestring);
        }
        cJSON *version = cJSON_GetObjectItem(meta, "version");
        if (version != NULL && cJSON_IsString(version)) {
            ruleset->version = strdup(version->valuestring);
        }
    }

    // Règles
    cJSON *rules_array = cJSON_GetObjectItem(json, "rules");
    if (rules_array != NULL && cJSON_IsArray(rules_array)) {
        int count = cJSON_GetArraySize(rules_array);
        ruleset->rule_count = count;
        ruleset->rules = (Rule*)malloc(count * sizeof(Rule));
        if (ruleset->rules == NULL) {
            free(ruleset);
            cJSON_Delete(json);
            return NULL;
        }

        for (int i = 0; i < count; i++) {
            cJSON *rule_json = cJSON_GetArrayItem(rules_array, i);
            Rule *rule = &ruleset->rules[i];
            rule->id = rule->category = rule->severity = NULL;
            rule->description = rule->check_type = rule->parameter = NULL;

            cJSON *field = cJSON_GetObjectItem(rule_json, "id");
            if (field != NULL && cJSON_IsString(field)) rule->id = strdup(field->valuestring);

            field = cJSON_GetObjectItem(rule_json, "category");
            if (field != NULL && cJSON_IsString(field)) rule->category = strdup(field->valuestring);

            field = cJSON_GetObjectItem(rule_json, "severity");
            if (field != NULL && cJSON_IsString(field)) rule->severity = strdup(field->valuestring);

            field = cJSON_GetObjectItem(rule_json, "description");
            if (field != NULL && cJSON_IsString(field)) rule->description = strdup(field->valuestring);

            field = cJSON_GetObjectItem(rule_json, "check_type");
            if (field != NULL && cJSON_IsString(field)) rule->check_type = strdup(field->valuestring);

            field = cJSON_GetObjectItem(rule_json, "parameter");
            if (field != NULL && cJSON_IsString(field)) rule->parameter = strdup(field->valuestring);
        }
    }

    cJSON_Delete(json);
    return ruleset;
}

void free_rules(RuleSet *ruleset) {
    if (ruleset == NULL) return;
    free(ruleset->document_type);
    free(ruleset->version);
    for (int i = 0; i < ruleset->rule_count; i++) {
        Rule *rule = &ruleset->rules[i];
        free(rule->id);
        free(rule->category);
        free(rule->severity);
        free(rule->description);
        free(rule->check_type);
        free(rule->parameter);
    }
    free(ruleset->rules);
    free(ruleset);
}

void print_rules(RuleSet *ruleset) {
    if (ruleset == NULL) {
        printf("RuleSet is NULL\n");
        return;
    }
    printf("=== Metadonnees ===\n");
    printf("Type: %s\n", ruleset->document_type ? ruleset->document_type : "NULL");
    printf("Version: %s\n", ruleset->version ? ruleset->version : "NULL");
    printf("\n=== Regles (%d) ===\n", ruleset->rule_count);
    for (int i = 0; i < ruleset->rule_count; i++) {
        Rule *rule = &ruleset->rules[i];
        printf("\nRegle %d:\n", i + 1);
        printf("  ID: %s\n", rule->id ? rule->id : "NULL");
        printf("  Categorie: %s\n", rule->category ? rule->category : "NULL");
        printf("  Severité: %s\n", rule->severity ? rule->severity : "NULL");
        printf("  Description: %s\n", rule->description ? rule->description : "NULL");
        printf("  Type: %s\n", rule->check_type ? rule->check_type : "NULL");
        printf("  Parametre: %s\n", rule->parameter ? rule->parameter : "NULL");
    }
}