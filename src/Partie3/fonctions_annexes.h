#ifndef FONCTIONS_ANNEXES_H
#define FONCTIONS_ANNEXES_H

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_CARACTERES 62 // nombre de caractères admissibles pour chaque valeur de la clé

char **clef; // tableau permettant de stocker les clés candidates

/**************************************/
/* Fonctions du programme BREAK_CODE1 */
/************************************ */

// Fonction qui vérifie si un caractère est valide (alphanumérique, espace ou ponctuation)
int estCharValide(int c);

// Fonction qui génère les caractères admissibles pour chaque position d'une clé en fonction d'un texte chiffré
void clefAdmissible(char *chiffre, char **clef, int taille_clef);

// Fonction qui génère le produit cartésien des ensembles de caractères admissibles pour la clé
void produitCartesien(char **sets, int *sizes, char *result, int index, int n, FILE *file, int taille_clef);

// Fonction qui effectue un chiffrement/déchiffrement XOR entre un message et une clé
void xor(const char *msg, unsigned char *key, char *crypted, int len_msg);


/**************************************/
/* Fonctions du programme BREAK_CODE3 */
/************************************ */

#define MAX_LIGNS 100 // Nombre maximum de lignes à lire par batch
#define MAX_LEN_LIGNS 40 // Longueur maximum d'une ligne
#define MAX_SIZE_WORDTAB 100
#define MAX_LONGUEUR_MOT 40

// Fonction pour decrypter un fichier par la méthode XOR
int xor_crypt_decrypt(const char *text, const char *destination, const char *key);

// Fonction pour rechercher un mot dans un dictionnaire
int search_in_dictionary(const char *word, const char *dictionary);

// Fonction pour prélever les 100 premiers mots d'un texte et les placer dans un tableau
char ** text_to_tab(const char * text, int * word_count);

// Fonction pour libérer la mémoire après text_to_tab
void free_word_tab(char **word_tab, int word_count);

char **read_keys(const char *file_path, int *keys_count);
#endif