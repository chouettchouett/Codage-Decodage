#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_CARACTERES 62 // nombre de caractères admissibles pour chaque valeur de la clé

/**************************************/
/* Fonctions du programme BREAK_CODE1 */
/************************************ */

// Fonction qui vérifie si un caractère est valide (alphanumérique, espace ou ponctuation)
int estCharValide(int c) {
    return isalnum(c) || isspace(c) || ispunct(c);
}

// Fonction qui génère les caractères admissibles pour chaque position d'une clé en fonction d'un texte chiffré
void clefAdmissible(char *chiffre, char **clef, int taille_clef) {
    // Ensemble des caractères potentiellement utilisables dans la clé
    char clefCharacters[MAX_CARACTERES] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int tailleChiffre = strlen(chiffre); // Longueur du texte chiffré

    // Parcourt chaque position de la clé
    for (int i = 0; i < taille_clef; i++) {
        int m = 0; // Compteur des caractères valides pour cette position

        // Parcourt les caractères admissibles pour la clé
        for (int k = 0; k < MAX_CARACTERES; k++) {
            // Indicateur de validité pour le caractère actuel
            int estValide = 1; 
            // Vérifie que ce caractère est valide pour toute la séquence correspondante dans le texte chiffré
            for (int l = 0; estValide && (i + taille_clef * l) < tailleChiffre; l++) {
                if (!estCharValide(clefCharacters[k] ^ chiffre[i + taille_clef * l])) {
                    estValide = 0;
                }
            }
            // Si le caractère est valide, on l'ajoute aux options possibles pour cette position
            if (estValide) {
                clef[i][m++] = clefCharacters[k];
            }
        }
    }
}

// Fonction qui génère le produit cartésien des ensembles de caractères admissibles pour la clé
void produitCartesien(char **sets, int *sizes, char *result, int index, int n, FILE *file, int taille_clef) {
    // Si on a parcouru toutes les positions, on écrit une clé complète dans le fichier
    if (index == n) {
        fwrite(result, sizeof(char), taille_clef, file);
        fputs("\n", file);
        return;
    }
    // Parcourt chaque caractère de l'ensemble pour la position actuelle
    for (int i = 0; i < sizes[index]; i++) {
        result[index] = sets[index][i]; // Affecte un caractère à la position actuelle
        produitCartesien(sets, sizes, result, index + 1, n, file, taille_clef); // Appelle récursivement pour la position suivante
    }
}

// Fonction qui effectue un chiffrement/déchiffrement XOR entre un message et une clé
void xor(const char *msg, unsigned char *key, char *crypted, int len_msg) {
    int len_key = strlen((char *)key); // Longueur de la clé
    int index_key = 0; // Indice courant dans la clé

    // Applique l'opération XOR pour chaque caractère du message
    for (int i = 0; i < len_msg; i++) {    
        crypted[i] = msg[i] ^ key[index_key];
        index_key = (index_key + 1) % len_key;
    }
    crypted[len_msg] = '\0'; // Ajout du caractère nul pour terminer la chaîne chiffrée
}


/**************************************/
/* Fonctions du programme BREAK_CODE3 */
/************************************ */

#define MAX_LIGNS 100 // Nombre maximum de lignes à lire par batch
#define MAX_LEN_LIGNS 40 // Longueur maximum d'une ligne
#define MAX_SIZE_WORDTAB 100 // Taille maximum du tableau de mots
#define MAX_LONGUEUR_MOT 40 // Longueur maximum d'un mot

// Fonction pour déchiffrer un fichier par la méthode XOR
int xor_crypt_decrypt(const char *text, const char *destination, const char *key) {
    FILE *file = fopen(text, "r"); // Ouverture du fichier texte en lecture
    if (file == NULL) {
        printf("Error : Can't open file '%s'.\n", text); // Erreur si le fichier n'est pas trouvé
        return 1;
    }
    FILE *destination_file = fopen(destination, "w"); // Ouverture du fichier de destination en écriture
    if (destination_file == NULL) {
        printf("Erreur : impossible d'ouvrir le fichier '%s'.\n", destination); // Erreur si le fichier de destination n'est pas trouvé
        fclose(file);
        return 2;
    }

    int index_key = 0; // Initialisation de l'index de la clé
    int len_key = strlen(key); // Longueur de la clé
    char carac; // Variable pour stocker le caractère courant
    // Boucle pour lire chaque caractère du fichier et appliquer le XOR avec la clé
    while ((carac = fgetc(file)) != EOF) {
        carac = carac ^ key[index_key]; // Application de l'opération XOR sur le caractère
        fwrite(&carac, sizeof(char), 1, destination_file); // Écriture du caractère chiffré/déchiffré dans le fichier de destination
        index_key++; // Passage au caractère suivant de la clé
        if (index_key >= len_key) {
            index_key = 0; // Réinitialisation de l'index de la clé si on dépasse sa longueur
        }
    }

    // Ajout d'un caractère nul pour marquer la fin du fichier
    fwrite("\0", sizeof(char), 1, destination_file);

    fclose(file); // Fermeture des fichiers ouverts
    fclose(destination_file);
    return 0; // Retourne 0 en cas de succès
}

// Fonction pour rechercher un mot dans un dictionnaire
int search_in_dictionary(const char *word, const char *dictionary) {
    FILE *file = fopen(dictionary, "r"); // Ouverture du fichier dictionnaire en lecture
    if (file == NULL) {
        printf("Error : impossible to open file '%s'.\n", dictionary); // Erreur si le fichier dictionnaire n'est pas trouvé
        return 3;
    }

    char *ligns_tab[MAX_LIGNS] = {NULL}; // Tableau pour stocker les lignes lues du fichier
    int count = 0; // Compteur des lignes lues
    int found = 0; // Indicateur de recherche du mot

    // Lecture des lignes par blocs de MAX_LIGNES
    while (!found) {
        count = 0;

        while (count < MAX_LIGNS) {
            char lign[MAX_LEN_LIGNS]; // Buffer pour une ligne du fichier
            // Lecture d'une ligne du fichier
            if (fgets(lign, sizeof(lign), file) == NULL) {
                break; // Fin du fichier
            }
            lign[strcspn(lign, "\n")] = '\0'; // Suppression du caractère de nouvelle ligne

            // Allocation ou réallocation de mémoire pour stocker la ligne
            if (ligns_tab[count] == NULL) {
                ligns_tab[count] = malloc(strlen(lign) + 1);
            } else {
                ligns_tab[count] = realloc(ligns_tab[count], strlen(lign) + 1);
            }

            if (ligns_tab[count] == NULL) {
                printf("Error : insufficient memory.\n");
                fclose(file);
                return 4; // Erreur d'allocation mémoire
            }

            strcpy(ligns_tab[count], lign); // Copie de la ligne dans le tableau
            // Comparaison de la ligne lue avec le mot recherché
            if (strcmp(ligns_tab[count], word) == 0) {
                found = 1; // Mot trouvé
                break;
            }
            count++;
        }

        if (feof(file)) {
            break; // Fin du fichier sans trouver le mot
        }
    }

    fclose(file); // Fermeture du fichier
    // Libération de la mémoire allouée pour le tableau des lignes
    for (int i = 0; i < MAX_LIGNS; i++) {
        free(ligns_tab[i]);
    }

    return found; // Retourne 1 si le mot est trouvé, 0 sinon
}

// Fonction pour extraire les 100 premiers mots d'un texte et les placer dans un tableau
char **text_to_tab(const char *text, int *word_count) {
    FILE *file = fopen(text,"r"); // Ouverture du fichier texte en lecture
    if (file == NULL) {
        printf("Error : Can't open file '%s'.\n", text); // Erreur si le fichier n'est pas trouvé
        return NULL;
    }

    // Allocation du tableau pour les mots
    char **word_tab = malloc(MAX_SIZE_WORDTAB * sizeof(char *));
    if (word_tab == NULL) {
        printf("Error : malloc failed.\n"); // Erreur d'allocation mémoire
        fclose(file);
        return NULL;
    }
    char word[MAX_LONGUEUR_MOT]; // Buffer pour chaque mot
    int index = 0;

    // Lecture des mots du fichier
    while (fscanf(file, "%39s", word) != EOF && index < MAX_SIZE_WORDTAB) {
        // Conversion du mot en minuscules
        for (int i = 0; word[i] != '\0'; i++) {
            word[i] = tolower(word[i]);
        }
        // Allocation de mémoire pour chaque mot
        word_tab[index] = malloc((strlen(word) + 1) * sizeof(char));
        if (word_tab[index] == NULL) {
            printf("Error : malloc failed.\n");
            fclose(file);
            for (int i = 0; i < index; i++) { // Libération de la mémoire allouée
                free(word_tab[i]);
            }
            free(word_tab);
            return NULL;
        }
        strcpy(word_tab[index], word); // Copie du mot dans le tableau
        index++;
    }

    fclose(file); // Fermeture du fichier
    *word_count = index; // Nombre de mots lus
    return word_tab; // Retourne le tableau de mots
}

// Fonction pour libérer la mémoire après l'appel à text_to_tab
void free_word_tab(char **word_tab, int word_count) {
    for (int i = 0; i < word_count; i++) {
        free(word_tab[i]); // Libération de chaque mot
    }
    free(word_tab); // Libération du tableau de pointeurs
}

// Fonction pour lire les clés depuis un fichier
char **read_keys(const char *file_path, int *keys_count) {
    FILE *file = fopen(file_path, "r"); // Ouverture du fichier des clés en lecture
    if (file == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier des clés '%s'.\n", file_path); // Erreur si le fichier n'est pas trouvé
        return NULL;
    }

    // Comptage du nombre de clés dans le fichier
    char line[256];
    *keys_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        (*keys_count)++;
    }
    rewind(file); // Rewind du fichier pour pouvoir le lire depuis le début

    // Allocation du tableau de clés
    char **keys_tab = malloc(*keys_count * sizeof(char *));
    if (keys_tab == NULL) {
        printf("Erreur : Allocation mémoire pour les clés.\n");
        fclose(file);
        return NULL;
    }

    // Lecture des clés et stockage dans le tableau
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Suppression du caractère '\n' de la clé
        keys_tab[i] = strdup(line); // Copier la clé dans le tableau
        if (keys_tab[i] == NULL) {
            printf("Erreur : Allocation mémoire pour une clé.\n");
            // Libération de la mémoire déjà allouée
            for (int j = 0; j < i; j++) {
                free(keys_tab[j]);
            }
            free(keys_tab);
            fclose(file);
            return NULL;
        }
        i++;
    }
    fclose(file); // Fermeture du fichier
    return keys_tab; // Retourne le tableau des clés
}