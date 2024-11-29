#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "break_code.h"
#include "tests_break_code.h"

// Macro pour assertions avec messages
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Échec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

// Gestion des fichiers temporaires
void cleanup_files(const char* files[], int file_count) {
    for (int i = 0; i < file_count; i++) {
        remove(files[i]);
    }
}

// Fonction utilitaire pour créer un fichier temporaire
void create_mock_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur à l'ouverture de %s\n", filename);
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

//-----------------------------
//
// Section C1 : Tests pour crack_c1
void test_crack_c1_invalid_key_length() {
    printf("=== Test Crack C1 : Longueur de Clé Invalide ===\n");

    const char* input_file = "test_c1_invalid_input.txt";
    const char* key_file = "test_c1_invalid_keys.txt";
    create_mock_file(input_file, "encryptedtext");

    int result = crack_c1(input_file, key_file, -1); // Longueur de clé invalide
    assert_message(result != 0, "Crack C1 aurait dû échouer avec une longueur de clé invalide.");

    const char* files[] = {input_file, key_file};
    cleanup_files(files, 2);
}

void test_crack_c1_missing_file() {
    printf("=== Test Crack C1 : Fichier Inexistant ===\n");

    const char* input_file = "nonexistent_file.txt";
    const char* key_file = "test_c1_missing_keys.txt";

    int result = crack_c1(input_file, key_file, 3);
    assert_message(result != 0, "Crack C1 aurait dû échouer avec un fichier inexistant.");

    const char* files[] = {key_file};
    cleanup_files(files, 1);
}

// Vérifie que les clés générées sont correctes
void verify_generated_keys(const char* key_file) {
    FILE* key_output = fopen(key_file, "r");
    assert_message(key_output != NULL, "Le fichier de clés n'a pas été généré.");
    char buffer[1024];
    fgets(buffer, sizeof(buffer), key_output);
    fclose(key_output);
    assert_message(strlen(buffer) > 0, "Aucune clé candidate générée.");
    printf("Clés générées : %s\n", buffer);
}

// Vérifie que le fichier de sortie décrypté est correct
void verify_decrypted_output(const char* output_file, const char* expected_content) {
    FILE* output = fopen(output_file, "r");
    assert_message(output != NULL, "Le fichier de sortie décrypté n'a pas été généré.");
    char content[256];
    fgets(content, sizeof(content), output);
    fclose(output);
    assert_message(strcmp(content, expected_content) == 0, "Le contenu décrypté est incorrect.");
    printf("Message décrypté : %s\n", content);
}

// Test principal pour crack_c1
void test_crack_c1_valid_input() {
    printf("=== Test Crack C1 : Entrée Valide ===\n");

    const char* input_file = "test_c1_input.txt";
    const char* key_file = "test_c1_keys.txt";
    const char* output_file = "test_c1_output.txt";

    // Préparer un fichier chiffré fictif
    create_mock_file(input_file, "test123encrypted");

    // Étape 1 : Appeler crack_c1
    int result = crack_c1(input_file, key_file, 3);
    assert_message(result == 0, "Crack C1 aurait dû réussir avec une entrée valide.");

    // Étape 2 : Vérifier les clés générées
    verify_generated_keys(key_file);

    // Étape 3 : Vérifier le fichier de sortie décrypté
    verify_decrypted_output(output_file, "expectedplaintext");

    // Nettoyage des fichiers
    const char* files[] = {input_file, key_file, output_file};
    cleanup_files(files, 3);
}

//--------------------------------------
//
// Section C2 : Tests pour crack_c2
void test_crack_c2_empty_file() {
    printf("=== Test Crack C2 : Fichier Vide ===\n");

    const char* input_file = "test_c2_empty.txt";
    const char* output_file = "test_c2_empty_output.txt";
    create_mock_file(input_file, ""); // Fichier vide

    int result = crack_c2(input_file, output_file, 3);
    assert_message(result != 0, "Crack C2 aurait dû échouer avec un fichier vide.");

    const char* files[] = {input_file, output_file};
    cleanup_files(files, 2);
}

void test_crack_c2_missing_file() {
    printf("=== Test Crack C2 : Fichier Inexistant ===\n");

    const char* input_file = "nonexistent_file.txt";
    const char* output_file = "test_c2_missing_output.txt";

    int result = crack_c2(input_file, output_file, 3);
    assert_message(result != 0, "Crack C2 aurait dû échouer avec un fichier inexistant.");

    const char* files[] = {output_file};
    cleanup_files(files, 1);
}

void verify_output_file(const char* output_file, const char* expected_content) {
    FILE* output = fopen(output_file, "r");
    assert_message(output != NULL, "Le fichier de sortie n'a pas été créé.");
    char content[256];
    fgets(content, sizeof(content), output);
    fclose(output);
    assert_message(strcmp(content, expected_content) == 0, "Le contenu du fichier de sortie est incorrect.");
    printf("Contenu du fichier de sortie : %s\n", content);
}

void test_crack_c2_valid_input() {
    printf("=== Test Crack C2 : Entrée Valide ===\n");

    const char* input_file = "test_c2_input.txt";
    const char* output_file = "test_c2_output.txt";

    create_mock_file(input_file, "encrypteddata456");

    // Étape 1 : Appeler crack_c2
    int result = crack_c2(input_file, output_file, 3);
    assert_message(result == 0, "Crack C2 aurait dû réussir avec une entrée valide.");

    // Étape 2 : Vérifier le fichier de sortie décrypté
    verify_output_file(output_file, "plaintextdata");

    // Nettoyage des fichiers
    const char* files[] = {input_file, output_file};
    cleanup_files(files, 2);
}

//Tests pour Analyse Fréquentielle
void test_analyze_frequency() {
    printf("=== Test Analyse Fréquentielle ===\n");

    char text[] = "AABBBCCCDDDDE";
    int *frequencies = analyze_frequency(text);

    assert_message(frequencies['A'] == 2, "La fréquence de 'A' est incorrecte.");
    assert_message(frequencies['B'] == 3, "La fréquence de 'B' est incorrecte.");
    assert_message(frequencies['D'] == 4, "La fréquence de 'D' est incorrecte.");

    free(frequencies);
    printf("Test analyse fréquentielle réussi.\n");
}

// --------------------------------------
//
// Section C3 : Tests pour crack_c3

void test_crack_c3_empty_dictionary() {
    printf("=== Test Crack C3 : Dictionnaire Vide ===\n");

    const char* input_file = "test_c3_input.txt";
    const char* output_file = "test_c3_empty_output.txt";
    const char* dictionary_file = "test_c3_empty_dict.txt";

    create_mock_file(input_file, "encryptedtext");
    create_mock_file(dictionary_file, ""); // Dictionnaire vide

    int result = crack_c3(input_file, output_file, dictionary_file, 3);
    assert_message(result != 0, "Crack C3 aurait dû échouer avec un dictionnaire vide.");

    verify_no_output_generated(output_file);

    const char* files[] = {input_file, output_file, dictionary_file};
    cleanup_files(files, 3);
}

void test_crack_c3_no_match_in_dictionary() {
    printf("=== Test Crack C3 : Pas de Correspondance ===\n");

    const char* input_file = "test_c3_input.txt";
    const char* output_file = "test_c3_nomatch_output.txt";
    const char* dictionary_file = "test_c3_nomatch_dict.txt";

    create_mock_file(input_file, "encryptedtext");
    create_mock_file(dictionary_file, "other\nwords\nhere\n");

    int result = crack_c3(input_file, output_file, dictionary_file, 3);
    assert_message(result != 0, "Crack C3 aurait dû échouer sans correspondance dans le dictionnaire.");

    verify_no_output_generated(output_file);

    const char* files[] = {input_file, output_file, dictionary_file};
    cleanup_files(files, 3);
}


void verify_decrypted_with_dict(const char* output_file, const char* expected_output) {
    FILE* output = fopen(output_file, "r");
    assert_message(output != NULL, "Le fichier de sortie n'a pas été généré.");
    char buffer[1024];
    fgets(buffer, sizeof(buffer), output);
    fclose(output);

    assert_message(strcmp(buffer, expected_output) == 0, "Le texte déchiffré ne correspond pas au résultat attendu.");
    printf("Texte déchiffré : %s\n", buffer);
}

void verify_no_output_generated(const char* output_file) {
    FILE* output = fopen(output_file, "r");
    assert_message(output == NULL, "Le fichier de sortie aurait dû ne pas être généré.");
    if (output != NULL) fclose(output);
}

void test_crack_c3_with_valid_dictionary() {
    printf("=== Test Crack C3 : Dictionnaire Valide ===\n");

    const char* input_file = "test_c3_input.txt";
    const char* output_file = "test_c3_output.txt";
    const char* dictionary_file = "test_c3_dict.txt";
    const char* expected_output = "testmessage";

    create_mock_file(input_file, "encryptedtext");
    create_mock_file(dictionary_file, "test\nword\ntestmessage\n");

    int result = crack_c3(input_file, output_file, dictionary_file, 3);
    assert_message(result == 0, "Crack C3 a échoué pour une entrée valide.");

    verify_decrypted_with_dict(output_file, expected_output);

    const char* files[] = {input_file, output_file, dictionary_file};
    cleanup_files(files, 3);
}
