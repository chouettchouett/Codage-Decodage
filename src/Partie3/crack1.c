#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"
#define ALPHABET_SIZE 62 // 52 lettres + 10 chiffres

// Vérifie si un caractère est alphanumérique, un espace ou une ponctuation
int is_valid_char(char c) {
    return isalnum(c) || ispunct(c) || isblank(c) || isspace(c);
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

// Fonction qui génère les candidats possibles pour chaque position de la clé
void find_possible_keys(const unsigned char *encrypted_msg, size_t msg_len, 
                        size_t key_len, unsigned char candidates[key_len][ALPHABET_SIZE + 1]) {
    const char *alphabet = ALPHABET;

    // Initialiser les candidats
    for (int i = 0; i < key_len; i++) {
        int pos = 0;
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            int is_valid = 1;  // Variable pour vérifier si un candidat est valide pour tous les indices multiples
            char candidate = alphabet[j];

            // Tester ce candidat sur tous les indices multiples de key_len
            for (int k = i; k < msg_len; k += key_len) {
                char decrypted = encrypted_msg[k] ^ candidate;  // Appliquer l'opération XOR
                // Si le caractère déchiffré n'est pas valide, le candidat échoue
                if (!is_valid_char(decrypted)) {
                    printf("Candidat '%c' échoué à l'indice %u\n", candidate, k);
                    is_valid = 0;
                    break;  // On arrête les tests dès qu'un caractère est invalide
                }
            }

            // Si le candidat est valide pour tous les indices multiples, on l'ajoute
            if (is_valid) {
                candidates[i][pos++] = candidate;
            }
        }
        candidates[i][pos] = '\0';  // Terminer la liste par un caractère nul
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <fichier chiffré> <taille de la clé>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    size_t key_len = atoi(argv[2]);

    if (key_len == 0) {
        printf("Erreur : la taille de la clé doit être un entier positif.\n");
        return 1;
    }

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
