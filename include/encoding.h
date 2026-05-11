#ifndef ENCODING_H
#define ENCODING_H

/**
 * Vérifie si un octet est le début d'un caractère UTF-8.
 * Indispensable pour éviter de couper un caractère multi-octets
 * lors d'une suppression ou d'un calcul de position.
 */

 static inline int is_utf8_start_byte(unsigned char c){
    // En UTF-8, les octets de début ont des bits de poids fort spécifiques
    // 0xxxxxxx : caractère ASCII (1 octet)
    // 110xxxxx : début d'un caractère sur 2 octets
    // 1110xxxx : début d'un caractère sur 3 octets
    // 11110xxx : début d'un caractère sur 4 octets
    return (c & 0xC0) != 0x80; // Les octets de continuation ont les bits 10xxxxxx
 }

#endif // ENCODING_H