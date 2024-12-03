#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define ALPHABET_SIZE 62 // 52 lettres + 10 chiffres
#define FREQ_LETTERS_FR "EeAaSsIiNnTtRrOoLlDdCcPpVvBbGgQqFfHhJjXxKkYyZzWw" // Lettres fréquemment utilisées en français (majuscules et minuscules)


// Vérifie si un caractère est alphanumérique, un espace ou une ponctuation
int is_valid_char(char c) {
    return isalnum((unsigned char)c) || isspace((unsigned char)c) || ispunct((unsigned char)c);
}

// Supprime les espaces d'une chaîne de caractères
void remove_spaces(char *str) {
    char *write_ptr = str;
    for (char *read_ptr = str; *read_ptr != '\0'; read_ptr++) {
        if (*read_ptr != ' ') {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
    }
    *write_ptr = '\0';
}

// Vérifie si le caractère déchiffré est susceptible d'être une lettre ou un espace commun
int is_suspected_char(char c) {
    return (strchr(FREQ_LETTERS_FR, c) != NULL || c == ' ');
}

// Génère une matrice des candidats possibles pour chaque position de clé
void find_possible_keys(const unsigned char *encrypted_msg, size_t msg_len, 
                        size_t key_len, unsigned char candidates[key_len][ALPHABET_SIZE + 1]) {
    const char *alphabet = ALPHABET;

    // Initialiser les candidats
    for (size_t i = 0; i < key_len; i++) {
        size_t pos = 0;
        for (size_t j = 0; j < ALPHABET_SIZE; j++) {
            // Calculer le caractère déchiffré
            char decrypted = encrypted_msg[i % msg_len] ^ alphabet[j];

            // Si le caractère déchiffré est valide et potentiellement fréquent, on l'ajoute à la liste
            if (is_valid_char(decrypted) && is_suspected_char(decrypted)) {
                candidates[i][pos++] = alphabet[j];
            }
        }
        candidates[i][pos] = '\0'; // Terminer la ligne par un caractère nul
    }
}

int main() {
    // Nom du fichier contenant le message chiffré
    const char *filename = "/home/victorunix/ut3/ProjetAvance/message_crypte.txt";

    // Ouvrir le fichier
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier %s.\n", filename);
        return 1;
    }

    // Lire le contenu du fichier
    char hex_input[1024];
    if (fgets(hex_input, sizeof(hex_input), file) == NULL) {
        printf("Erreur : lecture du fichier échouée ou fichier vide.\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    // Supprimer les espaces du message hexadécimal
    remove_spaces(hex_input);

    // Retirer le saut de ligne à la fin (si présent)
    size_t hex_len = strlen(hex_input);
    if (hex_input[hex_len - 1] == '\n') {
        hex_input[hex_len - 1] = '\0';
        hex_len--;
    }

    // Convertir l'entrée hexadécimale en tableau d'octets
    size_t msg_len = hex_len / 2;
    unsigned char *encrypted_msg = (unsigned char *)malloc(msg_len);
    if (encrypted_msg == NULL) {
        printf("Erreur : allocation de mémoire échouée.\n");
        return 1;
    }

    for (size_t i = 0; i < msg_len; i++) {
        sscanf(&hex_input[2 * i], "%2hhx", &encrypted_msg[i]);
    }

    // Définir la longueur de la clé
    size_t key_len = 3;

    // Créer un tableau pour les candidats
    unsigned char candidates[key_len][ALPHABET_SIZE + 1];

    // Trouver les clés possibles pour chaque position
    find_possible_keys(encrypted_msg, msg_len, key_len, candidates);

    // Afficher les résultats dans le format demandé sans apostrophes
    printf("\nCandidats pour chaque position de la clé :\n");
    size_t total_keys = 1; // Variable pour stocker le nombre total de clés possibles
    for (size_t i = 0; i < key_len; i++) {
        size_t num_candidates = 0; // Nombre de candidats pour la position i
        printf("clef[%zu] ∈ [", i);
        for (size_t j = 0; candidates[i][j] != '\0'; j++) {
            printf("%c", candidates[i][j]);
            if (candidates[i][j + 1] != '\0') {
                printf(", ");
            }
            num_candidates++; // Compter le nombre de candidats pour cette position
        }
        printf("]\n");

        total_keys *= num_candidates; // Multiplier le nombre total de clés possibles
    }

    // Afficher le nombre total de clés possibles
    printf("\nNombre total de clés possibles : %zu\n", total_keys);

    free(encrypted_msg);
    return 0;
}
