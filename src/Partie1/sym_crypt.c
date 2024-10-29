#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand( time( NULL ) );

    char message_filepath[] = "message.txt";
    char encrypted_message[] = "encrypted_message.txt";
    char uncrypted_message[] = "uncrypted_message.txt";

    char* vector = gen_key(4);

    cbc_crypt(message_filepath, vector, encrypted_message);
    printf("Crypting OK\n");
    cbc_uncrypt(encrypted_message, vector, uncrypted_message);
    printf("Uncrypting OK\n");
}