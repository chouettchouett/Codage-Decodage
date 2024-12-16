#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "fonctions_annexes.h"

// Fonction qui effectue un chiffrement XOR avec une clé donnée
void break_code1(const char *msg, const char *cle) {
    int taille_clef = strlen(cle);  // Longueur de la clé
    int sizes[taille_clef];        // Tableau pour stocker le nombre de caractères valides par position
    char result[taille_clef];      // Résultat temporaire pour générer les clés candidates
    memset(result, 0, sizeof(result)); // Initialise le tableau résultat avec des zéros

    int msg_length = strlen(msg);  // Longueur du message original
    char crypte[msg_length + 1];   // Tableau pour stocker le message chiffré
    char decrypte[msg_length + 1]; // Tableau pour stocker le message déchiffré

    // Affichage du message original
    printf("Message d'origine : %s\n", msg);

    // Chiffrement du message avec la clé en utilisant XOR
    xor(msg, (unsigned char *)cle, crypte, msg_length);

    // Affichage du message chiffré sous forme hexadécimale
    printf("Message crypte :");
    for (size_t i = 0; i < msg_length; i++) {
        printf("%02x", (unsigned char)crypte[i]);
    }
    printf("\n");

    // Ouvrir le fichier "message_crypte.txt" en mode écriture
    FILE *file_crypte = fopen("message_crypte.txt", "w");
    if (file_crypte == NULL) {
        perror("Erreur lors de l'ouverture du fichier de chiffrement");
        exit(EXIT_FAILURE);
    }

    // Écrire le message chiffré sous forme hexadécimale dans le fichier
    for (size_t i = 0; i < msg_length; i++) {
        fprintf(file_crypte, "%02x", (unsigned char)crypte[i]);
    }
    fprintf(file_crypte, "\n");  // Ajoute une nouvelle ligne après le message chiffré

    // Fermer le fichier après l'écriture
    fclose(file_crypte);

    // Déchiffrement du message pour vérification
    xor(crypte, (unsigned char *)cle, decrypte, msg_length);
    printf("Message decrypte : %s\n", decrypte);

    // Allocation dynamique pour stocker les caractères valides pour chaque position de la clé
    clef = (char **)malloc(taille_clef * sizeof(char *));
    for (int i = 0; i < taille_clef; i++) {
        clef[i] = (char *)calloc(MAX_CARACTERES, sizeof(char)); // Initialise chaque position avec des zéros
    }

    // Génération des caractères valides pour chaque position de la clé
    clefAdmissible(crypte, clef, taille_clef);

    // Affichage des options valides pour chaque position
    for (int i = 0; i < taille_clef; i++) {
        int nbre = 0;
        printf("clef[%d] : [", i);
        for (int k = 0; k < MAX_CARACTERES; k++) {
            if (clef[i][k] != '\0') { // Affiche uniquement les caractères valides
                printf("%c, ", clef[i][k]);
                nbre++;
            }
        }
        sizes[i] = nbre; // Enregistre le nombre de caractères valides pour cette position
        printf("]\n");
    }

    // Ouverture d'un fichier pour sauvegarder toutes les clés candidates
    FILE *file = fopen("clefs_candidates_c1.txt", "w");
    if (file == NULL) { // Vérification de l'ouverture du fichier
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Génération du produit cartésien des clés possibles et écriture dans le fichier
    produitCartesien(clef, sizes, result, 0, taille_clef, file, taille_clef);

    // Fermeture du fichier après l'écriture
    fclose(file);

    // Libération de la mémoire allouée dynamiquement
    for (int i = 0; i < taille_clef; i++) {
        free(clef[i]);
    }
    free(clef);
}

// Fonction principale
int main(int argc, char *argv[]) {
    // Vérifie si les arguments requis sont présents
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <texte_non_crypte.txt> <cle>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char *cle = argv[2]; // Récupère la clé depuis les arguments
    if (strlen(cle) == 0) { // Vérifie que la clé n'est pas vide
        fprintf(stderr, "Erreur : la clé ne peut pas être vide.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fd; // Pointeur de fichier pour lire le message
    char msg[1024]; // Tableau pour stocker le contenu du fichier

    // Ouverture du fichier contenant le message non chiffré
    if ((fd = fopen(argv[1], "r")) == NULL) {
        perror("Echec d'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    // Lecture du contenu du fichier
    if (fgets(msg, sizeof(msg), fd) == NULL) {
        perror("Echec de lecture du fichier");
        fclose(fd);
        exit(EXIT_FAILURE);
    }
    fclose(fd); // Fermeture du fichier après la lecture

    // Appelle la fonction pour casser le code en passant le message et la clé
    break_code1(msg, cle);
    return 0;
}
