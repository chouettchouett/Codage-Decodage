#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * @brief Macro pour afficher des messages d'erreur et arrêter le programme en cas d'échec d'une assertion.
 */
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Échec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

/**
 * @brief Crée un fichier fictif pour les tests.
 * 
 * @param filename Nom du fichier à créer.
 * @param content Contenu à écrire dans le fichier.
 */
void create_mock_file(const char* filename, const char* content);

/**
 * @brief Nettoie un ensemble de fichiers en les supprimant.
 * 
 * @param files Tableau de noms de fichiers.
 * @param count Nombre de fichiers dans le tableau.
 */
void cleanup_files(const char* files[], int count);

#endif // TEST_UTILS_H
