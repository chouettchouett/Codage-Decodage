#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <caractère>\n", argv[0]);
        return 1;
    }

    // Récupérer le caractère depuis les arguments
    unsigned char input_char = argv[1][0];

    // Liste alphanumérique
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charset_size = sizeof(charset) - 1; // Taille du tableau sans le '\0'

    printf("Résultats du XOR entre '%c' et chaque caractère de la liste :\n\n", input_char);

    // Effectuer le XOR pour chaque caractère du charset
    for (size_t i = 0; i < charset_size; i++) {
        unsigned char result = input_char ^ charset[i];

        // Vérifie si le résultat est un caractère imprimable
        char display_char = isprint(result) ? result : '.';

        printf("'%c' XOR '%c' = '%c' (0x%02x)\n", 
               input_char, charset[i], display_char, result);
    }

    return 0;
}
