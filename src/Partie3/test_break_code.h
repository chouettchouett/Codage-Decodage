/**
 * @file main_tests.h
 * @brief Tests pour la Partie 3 : Décryptage.
 * 
 * Ce fichier contient les déclarations de toutes les fonctions de test pour la Partie 3.
 * Les tests concernent le décryptage par force brute et dictionnaire, y compris les vérifications
 * pour les entrées valides et invalides.
 */

#ifndef MAIN_TESTS_H
#define MAIN_TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_break_code.h"
// remplacer les noms de fichier de victor #include "break_code.h"

/**
 * @brief Test du crack_c1 avec une longueur de clé invalide.
 * 
 * Ce test vérifie que le programme gère correctement une clé dont la 
 * longueur est incorrecte.
 */
void test_crack_c1_invalid_key_length();

/**
 * @brief Test du crack_c1 lorsque le fichier est manquant.
 * 
 * Ce test vérifie que le programme renvoie une erreur lorsque le fichier
 * à déchiffrer est manquant.
 */
void test_crack_c1_missing_file();

/**
 * @brief Test du crack_c1 avec des entrées valides.
 * 
 * Ce test vérifie que le programme peut correctement déchiffrer un fichier 
 * avec une clé valide.
 */
void test_crack_c1_valid_input();

/**
 * @brief Test du crack_c2 avec un fichier vide.
 * 
 * Ce test vérifie que le programme gère correctement un fichier vide.
 */
void test_crack_c2_empty_file();

/**
 * @brief Test du crack_c2 lorsque le fichier est manquant.
 * 
 * Ce test vérifie que le programme renvoie une erreur lorsque le fichier
 * à déchiffrer est manquant.
 */
void test_crack_c2_missing_file();

/**
 * @brief Test du crack_c2 avec une décryption valide.
 * 
 * Ce test vérifie que le programme peut correctement déchiffrer un fichier
 * avec une méthode d'analyse fréquentielle valide.
 */
void test_crack_c2_valid_decryption();

/**
 * @brief Test de l'analyse frequencielle  
 * 
 * Ce test vérifie que le bon fonctionnement de l'analyse fréquentielle
 */
void test_analyze_frequency();

/**
 * @brief Test du crack_c3 avec un dictionnaire valide.
 * 
 * Ce test vérifie que le programme peut correctement utiliser un dictionnaire 
 * pour décrypter un fichier.
 */
void test_crack_c3_with_dictionary();

/**
 * @brief Test du crack_c3 avec un dictionnaire vide.
 * 
 * Ce test vérifie que le programme renvoie une erreur lorsqu'aucun mot valide
 * n'est présent dans le dictionnaire.
 */
void test_crack_c3_empty_dictionary();

/**
 * @brief Test du crack_c3 lorsque le dictionnaire est manquant.
 * 
 * Ce test vérifie que le programme renvoie une erreur lorsque le dictionnaire
 * à utiliser pour la décryption est manquant.
 */
void test_crack_c3_no_match_in_dictionary();

#endif // MAIN_TESTS_H
