#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Fonction pour générer une clé alphanumérique aléatoire
void gen_key(unsigned char *key, size_t length) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t charset_size = sizeof(charset) - 1;

    // Initialise le générateur de nombres aléatoires
    srand(time(NULL));

    for (size_t i = 0; i < length; i++) {
        key[i] = charset[rand() % charset_size];
    }
    key[length] = '\0'; // Terminer la chaîne avec un caractère nul
}

// Fonction pour chiffrer et déchiffrer avec XOR
void xor_cipher(unsigned char *msg, unsigned char *key, size_t msg_len, size_t key_len) {
    for (size_t i = 0; i < msg_len; i++) {
        msg[i] ^= key[i % key_len];
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s <fichier_a_crypter> <fichier_de_sortie> <taille_de_clé>\n", argv[0]);
        return 1;
    }

    const char *input_file = argv[1];  // Fichier à crypter
    const char *output_file = argv[2]; // Fichier de sortie
    size_t key_len = atoi(argv[3]);   // Taille de la clé, convertie en entier

    if (key_len <= 0) {
        printf("La taille de la clé doit être un nombre positif.\n");
        return 1;
    }

    // Ouvrir le fichier d'entrée en lecture binaire
    FILE *file_in = fopen(input_file, "rb");
    if (file_in == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", input_file);
        return 1;
    }

    // Aller à la fin du fichier pour obtenir la taille
    fseek(file_in, 0, SEEK_END);
    size_t file_size = ftell(file_in);
    fseek(file_in, 0, SEEK_SET);  // Revenir au début du fichier

    // Lire le contenu du fichier dans un tableau
    unsigned char *msg = (unsigned char *)malloc(file_size * sizeof(unsigned char));
    if (msg == NULL) {
        printf("Échec de l'allocation de mémoire.\n");
        fclose(file_in);
        return 1;
    }
    fread(msg, sizeof(unsigned char), file_size, file_in);
    fclose(file_in);  // Fermer le fichier après lecture

    // Allouer dynamiquement un tableau pour la clé
    unsigned char *key = (unsigned char *)malloc((key_len + 1) * sizeof(unsigned char));
    if (key == NULL) {
        printf("Échec de l'allocation de mémoire pour la clé.\n");
        free(msg);
        return 1;
    }

    // Générer une clé aléatoire
    gen_key(key, key_len);
    printf("Clé générée : %s\n", key);

    printf("Message avant chiffrement :\n");
    for (size_t i = 0; i < file_size; i++) {
        printf("%c", msg[i]);  // Afficher le message avant chiffrement
    }
    printf("\n");

    // Chiffrer le message
    xor_cipher(msg, key, file_size, key_len);

    // Ouvrir le fichier de sortie en écriture binaire
    FILE *file_out = fopen(output_file, "wb");
    if (file_out == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s.\n", output_file);
        free(msg);
        free(key);
        return 1;
    }

    // Écrire la clé et le message chiffré dans le fichier de sortie
    for (size_t i = 0; i < file_size; i++) {
        fprintf(file_out, "%02x", msg[i]);
    }
    //fprintf(file_out, "\n");

    fclose(file_out);

    // Libérer la mémoire allouée
    free(msg);
    free(key);
    return 0;
}
