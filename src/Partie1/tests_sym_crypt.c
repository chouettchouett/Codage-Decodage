#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>  
#include "sym_crypt_func.h"

// Définition de la macro assert_message
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Échec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

void test_xor() {
    char message[] = "Hello World!";
    char* key = gen_key(strlen(message));
    printf("Message original : %s\n", message);
    
    // Cryptage
    assert_message(xor(message, key) == 0, "Échec du chiffrement XOR");
    printf("Message crypté avec XOR : %s\n", message);
    
    // Décryptage
    assert_message(xor(message, key) == 0, "Échec du déchiffrement XOR");
    printf("Message décrypté avec XOR : %s\n", message);
    
    free(key);
}

void test_mask_xor_crypt() {
    char message[] = "Test Mask XOR!";
    printf("Message original : %s\n", message);
    
    // Cryptage avec masque
    assert_message(mask_xor_crypt(message) == 0, "Échec du chiffrement mask XOR");
    printf("Message crypté avec mask XOR : %s\n", message);
    
    // Décryptage avec masque
    assert_message(mask_xor_uncrypt(message) == 0, "Échec du déchiffrement mask XOR");
    printf("Message décrypté avec mask XOR : %s\n", message);
}

void test_cbc_crypt() {
    char* init_vector = "initialvector123"; // 16 caractères pour CBC_MASK_LENGTH
    char* message_filepath = "./message.txt";
    char* encrypted_filepath = "/tmp/encrypted.txt";
    char* decrypted_filepath = "/tmp/decrypted.txt";

    // Création d’un fichier contenant le message à chiffrer
    FILE *f = fopen(message_filepath, "w");
    if (f == NULL) {
        perror("Erreur à l'ouverture de message.txt pour écriture");
        return;
    }
    fprintf(f, "Message pour test CBC.");
    fclose(f);

    // Cryptage CBC
    int crypt_result = cbc_crypt(message_filepath, init_vector, encrypted_filepath);
    assert_message(crypt_result == 0, "Erreur lors du chiffrement CBC");
    printf("Chiffrement CBC terminé.\n");
    
    printf("Message chiffré : ");
    FILE *f_enc = fopen(encrypted_filepath, "r");
    char c;
    while ((c = fgetc(f_enc)) != EOF) {
        putchar(c);
    }
    fclose(f_enc);
    printf("\n");

    // Déchiffrement CBC
    int uncrypt_result = cbc_uncrypt(encrypted_filepath, init_vector, decrypted_filepath);
    assert_message(uncrypt_result == 0, "Erreur lors du déchiffrement CBC");
    printf("Déchiffrement CBC terminé.\n");
    
    FILE *f_orig = fopen(message_filepath, "r");
    FILE *f_dec = fopen(decrypted_filepath, "r");
    
    printf("=== Contenu des fichiers pour comparaison ===\n");
    printf("Message d'origine : ");
    char c1;
    while ((c1 = fgetc(f_orig)) != EOF) {
        putchar(c1);
    }
    printf("\n");
    
    printf("Message déchiffré : ");
    char c2;
    while ((c2 = fgetc(f_dec)) != EOF) {
        putchar(c2);
    }
    printf("\n");
     
    // Réinitialisation des fichiers pour la comparaison caractère par caractère
    rewind(f_orig);
    rewind(f_dec);
    while ((c1 = fgetc(f_orig)) != EOF && (c2 = fgetc(f_dec)) != EOF) {
        assert_message(c1 == c2, "Le contenu des fichiers d'origine et déchiffré ne correspond pas.");
    }
    assert_message(c1 == c2, "Les fichiers n'ont pas la même longueur.");

    fclose(f_orig);
    fclose(f_dec);
    // Nettoyage des fichiers temporaires
    remove(message_filepath);
    remove(encrypted_filepath);
    remove(decrypted_filepath);
}

int main() {
    // Initialisation pour rand()
    srand((unsigned) time(NULL));
    printf("=== Test XOR simple ===\n");
    test_xor();
    printf("\n=== Test mask XOR crypt ===\n");
    test_mask_xor_crypt();
    printf("\n=== Test CBC crypt ===\n");
    test_cbc_crypt();
    
    printf("\nTous les tests sont réussis.\n");

    return 0;
}
