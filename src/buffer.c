#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GapBuffer* gb_create(size_t capacity) {
    GapBuffer* gb = malloc(sizeof(GapBuffer));
    gb->buffer = malloc(capacity);
    gb->size = capacity;
    gb->gap_start = 0;
    gb->gap_end = capacity; // Au début, tout le buffer est un "gap"
    return gb;
}

// Déplace le gap pour que l'insertion se fasse à la position 'pos'
void gb_move_gap(GapBuffer* gb, size_t pos) {
    if (pos == gb->gap_start) return;

    if (pos < gb->gap_start) {
        // Déplacement vers la gauche
        size_t num_to_move = gb->gap_start - pos;
        memmove(gb->buffer + gb->gap_end - num_to_move, 
                gb->buffer + pos, num_to_move);
        gb->gap_start -= num_to_move;
        gb->gap_end -= num_to_move;
    } else {
        // Déplacement vers la droite
        size_t num_to_move = pos - gb->gap_start;
        memmove(gb->buffer + gb->gap_start, 
                gb->buffer + gb->gap_end, num_to_move);
        gb->gap_start += num_to_move;
        gb->gap_end += num_to_move;
    }
}

void gb_insert(GapBuffer* gb, char c) {
    // Si le gap est vide, il faudrait normalement faire un realloc (Phase 2)
    if (gb->gap_start == gb->gap_end) return;

    gb->buffer[gb->gap_start] = c;
    gb->gap_start++;
}

// Fonction CRUCIALE pour Mbayo et David
// Elle reconstruit le texte sans le "trou" au milieu
char* gb_get_full_text(GapBuffer* gb) {
    size_t text_len = gb->gap_start + (gb->size - gb->gap_end);
    char* full_text = malloc(text_len + 1); // +1 pour le '\0' final

    // Partie avant le gap
    memcpy(full_text, gb->buffer, gb->gap_start);
    
    // Partie après le gap
    memcpy(full_text + gb->gap_start, 
           gb->buffer + gb->gap_end, 
           gb->size - gb->gap_end);

    full_text[text_len] = '\0'; // Fin de chaîne pour le C
    return full_text;
}

void gb_free(GapBuffer* gb) {
    free(gb->buffer);
    free(gb);
}