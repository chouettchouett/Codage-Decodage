#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "tests_sym_crypt.h"

//TOPO : les tests fonctionnent. Les codes fonctionnent 5 fois sur 6 environ

int main() {
    // Initialisation pour rand()
    srand(time(NULL));
    printf("=== Test XOR simple ===\n");
    test_xor();
    printf("\n=== Test mask XOR crypt ===\n");
    test_mask_xor_crypt();
    printf("\n=== Test CBC crypt ===\n");
    test_cbc_crypt();
    
    printf("\nTous les tests sont réussis.\n");

    char message_filepath[] = "message.txt";
    char encrypted_message[] = "encrypted_message.txt";
    char uncrypted_message[] = "uncrypted_message.txt";

    //char* vector = gen_key(4);
    char vector[] = "PeutetrequeNONon";
    printf("vector=%s\n", vector);

    cbc_crypt(message_filepath, vector, encrypted_message);
    printf("Crypting OK\n");
    cbc_uncrypt(encrypted_message, vector, uncrypted_message);
    printf("Uncrypting OK\n");
    return 0;
}
