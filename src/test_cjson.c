#include <stdio.h>
#include "cJSON.h"  // Inclure le header de cJSON

int main() {
    const char *json = "{\"test\":\"OK\"}";
    cJSON *root = cJSON_Parse(json);
    if (root == NULL) {
        printf("Erreur de parsing JSON.\n");
        return 1;
    }
    printf("cJSON fonctionne !\n");
    cJSON_Delete(root);
    return 0;
}