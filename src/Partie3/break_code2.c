#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define ALPHABET_SIZE 26 // nombre de lettres dans l'alphabet


// fréquences d'apparitions théoriques des lettres de l'alphabet dans les langues française et anglaise
double frequences_langue_francaise[ALPHABET_SIZE] = {
    14.715, 1.044, 3.183, 3.669, 17.194, 1.066, 0.866, 0.737, 7.529,
    0.613, 0.049, 5.456, 2.968, 7.095, 5.796, 2.521, 1.362, 6.553,
    7.948, 7.244, 6.311, 1.838, 0.049, 0.427, 0.128, 0.326 
};

double frequences_langue_anglaise[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
    0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987,
    6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
};


// fonction permettant de lire un fichier
char *read_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        perror("Erreur d'ouverture de fichier");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *content = malloc(size + 1);
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    return content;
}


// fonction permettant de calculer les fréquences d'apparitions des lettres
void calculer_frequences(const char *text, double frequences[ALPHABET_SIZE]) {
    int letter_counts[ALPHABET_SIZE] = {0};
    int total_letters = 0;

    for (int i = 0; text[i] != '\0'; i++) {
        if (text[i] >= 'a' && text[i] <= 'z') {
            letter_counts[text[i] - 'a']++;
            total_letters++;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            letter_counts[text[i] - 'A']++;
            total_letters++;
        }
    }

    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequences[i] = total_letters > 0 ? (double)letter_counts[i] / total_letters * 100.0 : 0.0;
    }
}


// fonction permettant de déchiffrer un texte avec une clé XOR
void decrypt_xor(const char *ciphertext, char *plaintext, const char *key) {
    int i;
    for (i = 0; ciphertext[i] != '\0'; i++) {
        plaintext[i] = ciphertext[i] ^ key[i % strlen(key)];
    }
    plaintext[i] = '\0';
}


// fonction permettant de calculer la distance entre deux tableaux de fréquences (théorique et expérimental)
double calculer_distance(const double freq1[ALPHABET_SIZE], const double freq2[ALPHABET_SIZE]) {
    double distance = 0.0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        distance += pow(freq1[i] - freq2[i], 2);
    }
    return sqrt(distance);
}


int main(int argc, char *argv []) {

    const char *ciphertext_path = "message.txt";
    const char *keys_file_path = "clefs_candidates_c1.txt";
    const char *output_file_path = "clefs_candidates_c2.txt"; 

    if (argc != 2){
        fprintf(stderr, "Usage: %s <langue_choisie>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *language = argv[1];

    if (strcmp(language, "francais") != 0 && strcmp(language, "anglais") != 0) {
        fprintf(stderr, "Problème rencontré lors du choix de la langue : seul le français et l'anglais sont disponibles.\n");
        exit(EXIT_FAILURE);
    }

    // on lit le texte chiffré
    char *ciphertext = read_file(ciphertext_path);
    if (ciphertext == NULL) {
        fprintf(stderr, "Erreur lors de la lecture du fichier ciphertext.txt\n");
        exit(EXIT_FAILURE);
    }

    // on lit les clés candidates provenant du Crack n°1
    FILE *keys_file = fopen(keys_file_path, "r");
    if (!keys_file) {
        perror("Erreur d'ouverture du fichier de clés");
        exit(EXIT_FAILURE);
    }

    char line[100]; // on définit une taille maximale pour la ligne du fichier
    char *key_str;
    int num_keys = 0;

    // on compte le nombre de clés
    while (fgets(line, sizeof(line), keys_file) != NULL) {
        num_keys++;
    }
    rewind(keys_file);

    // on alloue de la mémoire pour stocker les clés candidates dans un tableau
    char **keys = (char **)malloc(num_keys * sizeof(char *));
    if (keys == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de clés.\n");
        fclose(keys_file);
        free(ciphertext);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_keys; i++) {
        keys[i] = (char *)malloc(100); 
        if (keys[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour une clé.\n");

            for (int j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            fclose(keys_file);
            free(ciphertext);
            exit(1);
        }
    }

    // on lit les clés contenues dans le fichier 
    int i = 0;
    while (fgets(line, sizeof(line), keys_file) != NULL) {
        key_str = strtok(line, "\n"); 
        strcpy(keys[i], key_str);
        i++;
    }
    fclose(keys_file);

    int best_key_index = -1;
    double best_distance = INFINITY;

    // fichier de sortie, il collecte les résultats des nouvelles clefs candidates après le Crack n°2
    FILE *output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }

    // on lit et on teste chacune des clés
    for (int k = 0; k < num_keys; k++) {
        char *plaintext = malloc(strlen(ciphertext) + 1);

        // on déchiffre le texte avec la clé courante
        decrypt_xor(ciphertext, plaintext, keys[k]);

        // on cacule la fréquence d'apparition des lettres
        double frequences[ALPHABET_SIZE];
        calculer_frequences(plaintext, frequences);

        // on établit la différence de distance entre les valeurs expérimentales et théoriques
        double target_frequencies[ALPHABET_SIZE];
        if (strcmp(language, "francais") == 0) {
            memcpy(target_frequencies, frequences_langue_francaise, sizeof(frequences_langue_francaise));
        } else if (strcmp(language, "anglais") == 0) {
            memcpy(target_frequencies, frequences_langue_anglaise, sizeof(frequences_langue_anglaise));
        } else {
            fprintf(stderr, "Problème rencontré lors du choix de la langue : seul le français et l'anglais sont disponibles.\n");
            exit(EXIT_FAILURE);
        }
        double distance = calculer_distance(target_frequencies, frequences);
        
        // on écrit les résultats obtenues dans le fichier de sortie
        fprintf(output_file, "Clé: %s, Distance: %.4f\n", keys[k], distance);

        // on trie et on identifie quelle clé est la meilleure
        if (distance < best_distance) {
            best_distance = distance;
            best_key_index = k;
        }

        free(plaintext);
    }

    // on affiche la clé avec le meilleur score
    printf("Meilleure clé : %s\n", keys[best_key_index]);

    // on écrit la meilleure clé dans le fichier
    fprintf(output_file, "\nMeilleure clé : %s\n", keys[best_key_index]);

    // on lière la mémoire allouée
    for (int i = 0; i < num_keys; i++) {
        free(keys[i]);
    }
    free(keys);
    free(ciphertext);

    return 0;
}