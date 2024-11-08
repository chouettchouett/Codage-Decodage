#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tests_dh_gen.h"

// Macro d'assertion pour afficher les erreurs spécifiques
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Echec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

// Fonction de test pour l'option -o sans nom de fichier
void test_option_o_no_filename() {
    int result = system("./dh_gen_group -o");
    assert_message(result != 0, "L'option -o sans nom de fichier aurait dû échouer");
    printf("Test de l'option -o sans nom de fichier réussi.\n");
}

// Fonction de test pour l'option -o avec nom de fichier valide
void test_option_o() {
    int result = system("./dh_gen_group -o param.txt");
    assert_message(result == 0, "L'option -o avec nom de fichier valide a échoué");
    
    FILE *file = fopen("param.txt", "r");
    assert_message(file != NULL, "Fichier param.txt non créé avec l'option -o");
    fclose(file);
    printf("Test de l'option -o avec nom de fichier valide réussi.\n");
}

// Fonction de test pour l'option -h
void test_option_h() {
    int result = system("./dh_gen_group -h");
    assert_message(result == 0, "L'option -h aurait dû afficher l'aide sans erreur");
    printf("Test de l'option -h réussi.\n");
}

// Fonction de vérification que p est un nombre premier de Sophie Germain
int is_prime(int n) {
    if (n <= 1) return 0;            // Les nombres <= 1 ne sont pas premiers.
    if (n <= 3) return 1;            // Les nombres 2 et 3 sont premiers.
    if (n % 2 == 0 || n % 3 == 0) return 0; // Exclut les multiples de 2 et 3 pour une optimisation.
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0; // Exclut les multiples de i et i+2.
    }
    return 1;  // Si aucun facteur trouvé, n est premier.
}

int is_sophie_germain_prime(int p) {
    return is_prime(p) && is_prime(2 * p + 1);
}

void test_prime_sophie_germain() {
    FILE *file = fopen("param.txt", "r");
    assert_message(file != NULL, "Fichier param.txt introuvable pour lire p");
    
    int p;
    fscanf(file, "%d", &p);
    fclose(file);

    assert_message(is_sophie_germain_prime(p), "p n'est pas un nombre premier de Sophie Germain");
    printf("Test de validité de p comme nombre premier de Sophie Germain réussi.\n");
}

// Fonction de test pour vérifier que g est un générateur de Z/pZ
int is_generator(int g, int p) {
    if (g <= 1 || g >= p) return 0; // Vérifie que g est dans un intervalle valide (entre 1 et p-1)

    int seen[p];
    for (int i = 0; i < p; i++) seen[i] = 0;

    int value = 1;
    for (int k = 0; k < p - 1; k++) {
        value = (value * g) % p;
        if (seen[value] != 0) return 0; // Si la valeur est déjà vue, g n’est pas un générateur
        seen[value] = 1;
    }

    for (int i = 1; i < p; i++) {
        if (seen[i] == 0) return 0; // Vérifie que tous les éléments de 1 à p-1 sont générés
    }
    return 1;
}

void test_generator() {
    FILE *file = fopen("param.txt", "r");
    assert_message(file != NULL, "Fichier param.txt introuvable pour lire g");

    int p, g;
    fscanf(file, "%d %d", &p, &g);
    fclose(file);

    assert_message(is_generator(g, p), "g n'est pas un générateur pour Z/pZ");
    printf("Test de validité de g comme générateur réussi.\n");
}

