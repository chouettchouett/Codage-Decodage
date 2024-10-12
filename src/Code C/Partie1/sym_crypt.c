#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand( time( NULL ) );

    char* message = malloc(sizeof(char)*strlen("Bonjour tout le monde, comment allez-vous ?"));
    message = "Bonjour tout le monde, comment allez-vous ?";

    printf("%s\n", message);
    char* key = gen_key(16);
    printf("key generated = %s , length = %d\n", key, (int)strlen(key));
    xor(message, key);
    printf("%s\n", message);
    xor(message, key);
    printf("%s\n", message);
}