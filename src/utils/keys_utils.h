#ifndef KEYS_UTILS_H
#define KEYS_UTILS_H

#include <stdio.h>
#include <stdbool.h>

#define KEY_MAX_SIZE 100

// Génère une clef en utilisant la simulation de la partie 2 (-> key)
void gen_key_dh(char *key, FILE *log_file);

// Renvoie le nombre de clefs de la liste
int get_nb_keys(FILE *log_file);

// Renvoie la n-ième clef de la liste
char *get_key(int key_nb, FILE *log_file);

// Met à 1 l'utilisation de la n-ième clef de la liste
void set_key_used(int key_nb, char *key, FILE *log_file);

#endif