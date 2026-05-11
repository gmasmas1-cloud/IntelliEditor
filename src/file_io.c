#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "buffer.h"

// Fonction pour lire un fichier et le charger dans un GapBuffer
int gb_load_from_file(GapBuffer* gb, const char* path) {
    FILE* file = fopen(path, "rb"); // Lecteure binaire pour respecter l'UTF-8
    if (!file) return -1; // Erreur d'ouverture

    fseek(file, 0, SEEK_END);
    long fsize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* temp = malloc(fsize + 1); // +1 pour le '\0' final
    if (!temp) {
        fclose(file);
        return -2; // Erreur d'allocation
    }

    fread(temp, 1, fsize, file);
    fclose(file);
    temp[fsize] = '\0'; // Fin de chaîne pour le C

    // Insérer le contenu du fichier dans le GapBuffer
    gb->gap_start = 0;
    gb->gap_end = gb->size; // Tout le buffer est un gap au début

    for(long i = 0; i < fsize; i++) {
        gb_insert(gb, temp[i]);
    }
    free(temp);
    return 0; // Succès
}

// Fonction pour sauvegarder le contenu d'un GapBuffer dans un fichier
int gb_save_to_file(GapBuffer* gb, const char* path) {
    FILE* file = fopen(path, "wb"); // Écriture binaire pour respecter l'UTF-8
    if (!file) return -1; // Erreur d'ouverture

    char* content = gb_get_full_text(gb);
    if (!content) {
        fclose(file);
        return -2; // Erreur d'allocation
    }

    size_t text_len = strlen(content);
    fwrite(content, 1, text_len, file);

    free(content);
    fclose(file);
    return 0; // Succès

}