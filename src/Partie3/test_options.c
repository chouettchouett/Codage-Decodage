#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../utils/test_utils.h"
//#include "break_code.h"

//ajout en cathastrophe :
/**
 * @brief Analyse les options passées en ligne de commande.
 * @param argc Nombre d'arguments.
 * @param argv Tableau des arguments.
 * @return 0 en cas de succès, -1 en cas d'erreur.
 */
int parse_options(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Erreur : Aucun argument fourni. Utilisez -h pour afficher l'aide.\n");
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            printf("Usage : break_code [OPTIONS]\n");
            printf("  -i fichier    : Fichier à décrypter\n");
            printf("  -m c1/all     : Méthode de décryptage (c1 ou all)\n");
            printf("  -k longueur   : Longueur de la clé\n");
            printf("  -d fichier    : Fichier dictionnaire\n");
            printf("  -l fichier    : Fichier de log\n");
            printf("  -h            : Affiche cette aide\n");
            return 0; // Aide affichée avec succès
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            printf("Option -i : Fichier spécifié : %s\n", argv[++i]);
        } else if (strcmp(argv[i], "-m") == 0 && i + 1 < argc) {
            printf("Option -m : Méthode spécifiée : %s\n", argv[++i]);
        } else if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            printf("Option -k : Longueur de clé spécifiée : %s\n", argv[++i]);
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            printf("Option -d : Fichier dictionnaire spécifié : %s\n", argv[++i]);
        } else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc) {
            printf("Option -l : Fichier de log spécifié : %s\n", argv[++i]);
        } else {
            fprintf(stderr, "Erreur : Option inconnue ou argument manquant pour '%s'. Utilisez -h pour l'aide.\n", argv[i]);
            return -1;
        }
    }
    return 0; // Succès
}

void test_help_option() {
    printf("=== Test Option -h ===\n");
    char* args[] = {"break_code", "-h", NULL};
    int result = parse_options(2, args);
    assert_message(result == 0, "L'option -h n'a pas fonctionné comme prévu.");
    printf("Option -h testée avec succès.\n");
}

void test_input_file_option() {
    printf("=== Test Option -i ===\n");
    const char* input_file = "test_input.txt";
    create_mock_file(input_file, "dummy content");

    char* args[] = {"break_code", "-i", (char*)input_file, NULL};
    int result = parse_options(3, args);
    assert_message(result == 0, "L'option -i n'a pas fonctionné comme prévu.");

    cleanup_files((const char*[]){input_file}, 1);
    printf("Option -i testée avec succès.\n");
}

void test_method_option() {
    printf("=== Test Option -m ===\n");
    char* args1[] = {"break_code", "-m", "c1", NULL};
    int result1 = parse_options(3, args1);
    assert_message(result1 == 0, "L'option -m c1 n'a pas fonctionné comme prévu.");

    char* args2[] = {"break_code", "-m", "all", NULL};
    int result2 = parse_options(3, args2);
    assert_message(result2 == 0, "L'option -m all n'a pas fonctionné comme prévu.");
    printf("Option -m testée avec succès.\n");
}

void test_key_length_option() {
    printf("=== Test Option -k ===\n");
    char* args[] = {"break_code", "-k", "4", NULL};
    int result = parse_options(3, args);
    assert_message(result == 0, "L'option -k n'a pas fonctionné comme prévu.");
    printf("Option -k testée avec succès.\n");
}

void test_dictionary_option() {
    printf("=== Test Option -d ===\n");
    const char* dictionary_file = "test_dict.txt";
    create_mock_file(dictionary_file, "word1\nword2\nword3");

    char* args[] = {"break_code", "-d", (char*)dictionary_file, NULL};
    int result = parse_options(3, args);
    assert_message(result == 0, "L'option -d n'a pas fonctionné comme prévu.");

    cleanup_files((const char*[]){dictionary_file}, 1);
    printf("Option -d testée avec succès.\n");
}

void test_log_file_option() {
    printf("=== Test Option -l ===\n");
    const char* log_file = "test_log.txt";

    char* args[] = {"break_code", "-l", (char*)log_file, NULL};
    int result = parse_options(3, args);
    assert_message(result == 0, "L'option -l n'a pas fonctionné comme prévu.");

    // Vérification de l'existence du fichier
    FILE* log = fopen(log_file, "r");
    assert_message(log != NULL, "Le fichier de log n'a pas été créé.");
    fclose(log);

    cleanup_files((const char*[]){log_file}, 1);
    printf("Option -l testée avec succès.\n");
}
