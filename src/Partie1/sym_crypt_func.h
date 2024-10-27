#ifndef __FUNC_SYM_CRYPT__
#define __FUNC_SYM_CRYPT__

#define M_CRYPT 1;
#define M_DECRYPT 2;

/*
* Generates an alphanumeric key of variable "length"
*/
char* gen_key(int length);


void xor(char* message, char* key);


void mask_xor_crypt(char* message);


void mask_xor_uncrypt(char* message);


void cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath );


void cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* message_filepath );

#endif