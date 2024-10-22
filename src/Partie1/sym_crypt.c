#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand( time( NULL ) );

    char message[8] = "Bonjour";

    printf("Initial message : %s\n", message);
    mask_xor(message, M_CRYPT);

    mask_xor(message, M_DECRYPT);
    printf("Decrypted message : %s\n", message);
}