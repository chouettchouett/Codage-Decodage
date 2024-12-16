#include <stdio.h>       
#include <stdlib.h>      
#include <string.h>      
#include "tad_liste.h"  

// Fonction pour créer un nouveau nœud de la liste
Node* create_node(const char* key, int score) {
    // Allocation mémoire pour un nouveau nœud
    Node* newNode = (Node*)malloc(sizeof(Node)); 
    if (newNode == NULL) { // Si l'allocation échoue
        printf("Erreur : allocation mémoire échouée.\n");
        exit(1); // Sortie du programme avec un code d'erreur
    }
    
    // Copie sécurisée de la clé dans le nœud
    strncpy(newNode->elem.key, key, sizeof(newNode->elem.key) - 1); 
    newNode->elem.key[sizeof(newNode->elem.key) - 1] = '\0';  // Terminaison de la chaîne de caractères
    newNode->elem.score = score;  // Initialisation du score du nœud
    newNode->next = NULL;         // Initialisation du pointeur suivant à NULL, car ce sera le dernier élément pour l'instant
    
    return newNode;  // Retour du nœud créé
}

// Fonction pour insérer un nœud dans une liste triée par score décroissant
Node* add_node(Node* head, const char* key, int score) {
    Node* newNode = create_node(key, score); // Création d'un nouveau nœud avec la clé et le score donnés

    // Cas où la liste est vide ou si le score du nouveau nœud est supérieur au score du premier élément
    if (head == NULL || score > head->elem.score) {
        newNode->next = head;  // Le nouveau nœud devient la tête de la liste
        return newNode;
    }

    // Parcours de la liste pour trouver la bonne position d'insertion
    Node* temp = head;
    while (temp->next != NULL && temp->next->elem.score >= score) { 
        temp = temp->next;  // Avancer dans la liste jusqu'à trouver le bon endroit
    }

    // Insertion du nouveau nœud à la bonne position dans la liste triée
    newNode->next = temp->next;
    temp->next = newNode;

    return head;  // Retourner la tête de la liste (qui reste inchangée si l'insertion a eu lieu au milieu)
}

// Fonction pour afficher tous les éléments de la liste
void print_list(Node* head) {
    Node* temp = head;
    while (temp != NULL) {  // Tant qu'il y a des éléments dans la liste
        printf("Key: %s, Score: %d\n", temp->elem.key, temp->elem.score);  // Afficher la clé et le score du nœud
        temp = temp->next;  // Passer au nœud suivant
    }
}

// Fonction pour libérer la mémoire allouée à la liste
void free_list(Node* head) {
    Node* temp;
    while (head != NULL) {  // Tant qu'il y a des éléments dans la liste
        temp = head;     // Sauvegarder le nœud actuel
        head = head->next;  // Passer au nœud suivant
        free(temp);         // Libérer la mémoire du nœud
    }
}