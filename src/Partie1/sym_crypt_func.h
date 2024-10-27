#ifndef __FUNC_SYM_CRYPT__
#define __FUNC_SYM_CRYPT__

/*
* Generates an alphanumeric key of variable "length"
*/
char* gen_key(int length);


int xor(char* message, char* key);


int mask_xor_crypt(char* message);


int mask_xor_uncrypt(char* message);


int cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath );


int cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* message_filepath );

#endif