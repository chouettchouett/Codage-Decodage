#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Définition de la taille de l'alphabet (26 lettres)
#define ALPHABET_SIZE 26

// Fréquences théoriques des lettres dans les langues française et anglaise
double frequences_langue_francaise[ALPHABET_SIZE] = {
    14.715, 1.044, 3.183, 3.669, 17.194, 1.066, 0.866, 0.737, 7.529,
    0.613, 0.049, 5.456, 2.968, 7.095, 5.796, 2.521, 1.362, 6.553,
    7.948, 7.244, 6.311, 1.838, 0.049, 0.427, 0.128, 0.326 
}; // 106.687.

double frequences_langue_anglaise[ALPHABET_SIZE] = {
    8.167, 1.492, 2.782, 4.253, 12.702, 2.228, 2.015, 6.094, 6.966,
    0.153, 0.772, 4.025, 2.406, 6.749, 7.507, 1.929, 0.095, 5.987,
    6.327, 9.056, 2.758, 0.978, 2.360, 0.150, 1.974, 0.074
}; // 99.999.

// Fonction pour lire un fichier et renvoyer son contenu sous forme de chaîne de caractères
char *read_file(const char *file_path) {
    FILE *file = fopen(file_path, "r"); // Ouverture du fichier en lecture
    if (!file) {
        perror("Erreur d'ouverture de fichier"); // Gestion de l'erreur si le fichier n'est pas ouvert
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END); // Déplacement du curseur à la fin du fichier
    long size = ftell(file);  // Taille du fichier
    rewind(file); // Repositionnement du curseur au début du fichier

    char *content = malloc(size + 1); // Allocation dynamique de mémoire pour le contenu du fichier
    fread(content, 1, size, file); // Lecture du fichier
    content[size] = '\0'; // Ajout du caractère nul pour terminer la chaîne
    fclose(file); // Fermeture du fichier
    return content; // Retourne le contenu du fichier
}

// Fonction pour calculer les fréquences d'apparition des lettres dans un texte
void calculer_frequences(const char *text, double frequences[ALPHABET_SIZE]) {
    int letter_counts[ALPHABET_SIZE] = {0}; // Tableau pour compter les occurrences des lettres
    int total_letters = 0; // Compteur du nombre total de lettres

    for (int i = 0; text[i] != '\0'; i++) {
        // Vérifie si le caractère est une lettre (minuscule ou majuscule)
        if (text[i] >= 'a' && text[i] <= 'z') {
            letter_counts[text[i] - 'a']++; // Incrémentation du compteur pour la lettre en minuscule
            total_letters++;
        } else if (text[i] >= 'A' && text[i] <= 'Z') {
            letter_counts[text[i] - 'A']++; // Incrémentation du compteur pour la lettre en majuscule
            total_letters++;
        }
    }

    // Calcul des fréquences des lettres en pourcentage
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        frequences[i] = total_letters > 0 ? (double)letter_counts[i] / total_letters * 100.0 : 0.0;
    }
}

// Fonction pour déchiffrer un texte chiffré avec une clé XOR
void decrypt_xor(const char *ciphertext, char *plaintext, const char *key) {
    int i;
    // Déchiffre chaque caractère du texte chiffré avec l'opération XOR
    for (i = 0; ciphertext[i] != '\0'; i++) {
        plaintext[i] = ciphertext[i] ^ key[i % strlen(key)]; // Utilisation de la clé de manière répétée
    }
    plaintext[i] = '\0'; // Terminaison de la chaîne
}

// Fonction pour calculer la distance entre deux distributions de fréquences
double calculer_distance(const double freq1[ALPHABET_SIZE], const double freq2[ALPHABET_SIZE]) {
    double distance = 0.0;
    // Calcul de la distance euclidienne entre les deux distributions
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        distance += pow(freq1[i] - freq2[i], 2); // Différence au carré pour chaque lettre
    }
    return sqrt(distance); // Retourne la racine carrée de la somme des carrés des différences
}

int main(int argc, char *argv []) {
    const char *ciphertext_path = "message_crypte.txt"; // Chemin du fichier contenant le texte chiffré
    const char *keys_file_path = "clefs_candidates_c1.txt"; // Chemin du fichier contenant les clés candidates
    const char *output_file_path = "clefs_candidates_c2.txt"; // Fichier pour enregistrer les résultats

    // Vérification des arguments en ligne de commande
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <langue_choisie>\n", argv[0]); // Affiche un message d'erreur si les arguments sont incorrects
        exit(EXIT_FAILURE);
    }

    const char *language = argv[1]; // Langue choisie (français ou anglais)

    // Vérifie si la langue choisie est valide
    if (strcmp(language, "francais") != 0 && strcmp(language, "anglais") != 0) {
        fprintf(stderr, "Problème rencontré lors du choix de la langue : seul le français et l'anglais sont disponibles.\n");
        exit(EXIT_FAILURE);
    }

    // Lecture du texte chiffré
    char *ciphertext = read_file(ciphertext_path);
    if (ciphertext == NULL) {
        fprintf(stderr, "Erreur lors de la lecture du fichier ciphertext.txt\n");
        exit(EXIT_FAILURE);
    }

    // Lecture du fichier des clés candidates
    FILE *keys_file = fopen(keys_file_path, "r");
    if (!keys_file) {
        perror("Erreur d'ouverture du fichier de clés");
        exit(EXIT_FAILURE);
    }

    char line[100]; // Taille maximale d'une ligne dans le fichier
    char *key_str;
    int num_keys = 0;

    // Comptage du nombre de clés dans le fichier
    while (fgets(line, sizeof(line), keys_file) != NULL) {
        num_keys++;
    }
    rewind(keys_file); // Réinitialisation du curseur de lecture du fichier

    // Allocation de mémoire pour les clés candidates
    char **keys = (char **)malloc(num_keys * sizeof(char *));
    if (keys == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire pour le tableau de clés.\n");
        fclose(keys_file);
        free(ciphertext);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_keys; i++) {
        keys[i] = (char *)malloc(100); // Allocation mémoire pour chaque clé
        if (keys[i] == NULL) {
            fprintf(stderr, "Erreur d'allocation mémoire pour une clé.\n");

            // Libération de la mémoire en cas d'erreur d'allocation
            for (int j = 0; j < i; j++) {
                free(keys[j]);
            }
            free(keys);
            fclose(keys_file);
            free(ciphertext);
            exit(1);
        }
    }

    // Lecture des clés dans le fichier
    int i = 0;
    while (fgets(line, sizeof(line), keys_file) != NULL) {
        key_str = strtok(line, "\n"); // Suppression du caractère de nouvelle ligne
        strcpy(keys[i], key_str); // Copie de la clé dans le tableau
        i++;
    }
    fclose(keys_file);

    int best_key_index = -1; // Indice de la meilleure clé
    double best_distance = INFINITY; // Initialisation de la distance minimale à l'infini

    // Ouverture du fichier de sortie pour enregistrer les résultats
    FILE *output_file = fopen(output_file_path, "w");
    if (output_file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier de sortie\n");
        exit(1);
    }

    // Test de chaque clé candidate
    for (int k = 0; k < num_keys; k++) {
        char *plaintext = malloc(strlen(ciphertext) + 1); // Allocation de mémoire pour le texte déchiffré

        // Déchiffrement avec la clé actuelle
        decrypt_xor(ciphertext, plaintext, keys[k]);

        // Calcul des fréquences pour le texte déchiffré
        double freq_decrypted[ALPHABET_SIZE] = {0};
        calculer_frequences(plaintext, freq_decrypted);

        // Choix des fréquences de référence en fonction de la langue
        double *reference_freq;
        if (strcmp(language, "francais") == 0) {
            reference_freq = frequences_langue_francaise;
        } else {
            reference_freq = frequences_langue_anglaise;
        }

        // Calcul de la distance entre la distribution des fréquences du texte déchiffré et la langue choisie
        double distance = calculer_distance(freq_decrypted, reference_freq);

        // Si la distance est meilleure que la précédente, on met à jour la meilleure clé
        if (distance < best_distance) {
            best_distance = distance;
            best_key_index = k;
        }

        // Écriture de la clé candidate et de la distance dans le fichier de sortie
        fprintf(output_file, "%s\n", keys[k]);
        // fprintf(output_file, "%s : %f\n", keys[k], distance);

        free(plaintext); // Libération de la mémoire pour le texte déchiffré
    }

    // Si une clé valide a été trouvée, afficher et écrire la meilleure clé
    if (best_key_index != -1) {
        printf("La meilleure clé est : %s\n", keys[best_key_index]);
    } else {
        fprintf(output_file, "\nAucune clé valide trouvée.\n");
    }

    // Libération de la mémoire et fermeture des fichiers
    for (int i = 0; i < num_keys; i++) {
        free(keys[i]);
    }
    free(keys);
    fclose(output_file);
    free(ciphertext);
    
    return 0;
}