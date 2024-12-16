#ifndef ORDONERED_LIST_H
#define ORDONERED_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure représentant un élément avec une clé et un score
typedef struct Element {
    char key[50]; // Une clé de type chaîne de caractères avec une taille maximale de 49 caractères
    int score;    // Un score de type entier, associé à la clé
} Element;

// Structure représentant un nœud de la liste chaînée
typedef struct Node {
    Element elem;         // L'élément (clé et score) stocké dans ce nœud
    struct Node* next;    // Un pointeur vers le nœud suivant dans la liste chaînée
} Node;

// Fonction pour créer un nouveau nœud de la liste
Node* create_node(const char* key, int score);

// Fonction pour insérer un nœud dans une liste triée par score décroissant
Node* add_node(Node* head, const char* key, int score);

// Fonction pour afficher tous les éléments de la liste
void print_list(Node* head);

// Fonction pour libérer la mémoire allouée à la liste
void free_list(Node* head);


#endif