// src/test_cjson.c
#include <stdio.h>   // Pour printf()
#include "cJSON.h"   // Bibliothèque JSON

int main() {
    // 1. Chaîne JSON de test (similaire aux règles du projet)
    const char *json_test = "{\"id\": \"R001\", \"description\": \"Test de règle\"}";

    // 2. Parser le JSON (convertir la chaîne en objet C)
    cJSON *rules = cJSON_Parse(json_test);
    if (rules == NULL) {
        printf("Le JSON est invalide !\n");
        return 1;  // Quitte avec erreur
    }

    // 3. Extraire des données
    cJSON *id = cJSON_GetObjectItem(rules, "id");
    cJSON *desc = cJSON_GetObjectItem(rules, "description");

    if (id != NULL && cJSON_IsString(id)) {
        printf("ID: %s\n", id->valuestring);  // Affiche "R001"
    }
    if (desc != NULL && cJSON_IsString(desc)) {
        printf("Description: %s\n", desc->valuestring);  // Affiche "Test de règle"
    }

    // 4. Libérer la mémoire (TRÈS IMPORTANT en C !)
    cJSON_Delete(rules);
    return 0;
}