#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tests_dh_gen.h"
#include "../utils/test_utils.h" //assert_message

// Fonction de test pour l'option -o sans nom de fichier
void test_option_o_no_filename() {
    int result = system("./dh_gen_group -o > /dev/null"); // "> /dev/null" pour ne pas afficher la sortie de dh_gen_group durant le test
    assert_message(result != 0, "L'option -o sans nom de fichier aurait dû échouer");
    printf("Test de l'option -o sans nom de fichier réussi.\n");
}

// Fonction de test pour l'option -o avec nom de fichier valide
void test_option_o() {
    int result = system("./dh_gen_group -o param.txt");
    assert_message(result == 0, "L'option -o avec nom de fichier valide a échoué");
    
    FILE *file = fopen("tmp/param.txt", "r");
    assert_message(file != NULL, "Fichier param.txt non créé avec l'option -o");
    fclose(file);
    printf("Test de l'option -o avec nom de fichier valide réussi.\n");
}

// Fonction de test pour l'option -h
void test_option_h() {
    int result = system("./dh_gen_group -h > /dev/null"); // "> /dev/null" pour ne pas afficher la sortie de dh_gen_group durant le test
    assert_message(result == 0, "L'option -h aurait dû afficher l'aide sans erreur");
    printf("Test de l'option -h réussi.\n");
}

// L'ensemble des fonctions du fichiers dh_prime qu'on utilise sont déjà vérifié, rabin, test_prime...
