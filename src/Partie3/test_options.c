#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "test_utils.h"
#include "break_code.h"

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
