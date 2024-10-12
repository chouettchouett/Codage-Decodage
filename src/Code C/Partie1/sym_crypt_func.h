#ifndef __FUNC_SYM_CRYPT__
#define __FUNC_SYM_CRYPT__

/*
* Generates an alphanumeric key of variable "length"
*/
char* gen_key(int length);


void xor(char* message, char* key);

/*
* Apply a Xor operation on the "message" using a mask generated with the "key", 
* the lastest mask_xor mask is visible in the mask.txt file
*/
void mask_xor_crypt(char* message);


void mask_xor_uncrypt(char* message);


void cbc_crypt(char* message, char* init_vector, char* key);


void cbc_uncrypt(char* message, char* init_vector, char* key);

#endif 