#ifndef TEST_UTILS_H
#define TEST_UTILS_H

/**
 * @brief Macro pour afficher des messages d'erreur et arrêter le programme en cas d'échec d'une assertion.
 */
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Échec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

#endif
