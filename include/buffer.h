#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>

typedef struct {
    char* buffer;      // Le tableau de caractères
    size_t size;       // Taille totale (données + gap)
    size_t gap_start;  // Début du trou
    size_t gap_end;    // Fin du trou
} GapBuffer;

// Fonctions de base
GapBuffer* gb_create(size_t capacity);
void gb_insert(GapBuffer* gb, char c);
void gb_delete(GapBuffer* gb);
void gb_move_gap(GapBuffer* gb, size_t pos);

// Fonctions pour Mbayo (DEV-B) et David (DEV-C)
char* gb_get_full_text(GapBuffer* gb);

// Nettoyage
void gb_free(GapBuffer* gb);

#endif // BUFFER_H