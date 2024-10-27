#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand( time( NULL ) );

    char message[8] = "Bonjour";

    printf("Initial message : %s\n", message);
    mask_xor_crypt(message);
    printf("Crypting OK\n");
    mask_xor_uncrypt(message);
    printf("Uncrypting OK\n");
    printf("Decrypted message : %s\n", message);
}