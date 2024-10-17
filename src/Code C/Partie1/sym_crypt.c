#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand( time( NULL ) );

    char message[8] = "Bonjour";

    printf("Initial message : %s\n", message);
    char* key = gen_key(16);
    printf("Key generated = %s\n", key);
    xor(message, key);

    xor(message, key);
    printf("Decrypted message : %s\n", message);
}