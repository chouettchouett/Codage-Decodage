#ifndef __FUNC_SYM_CRYPT__
#define __FUNC_SYM_CRYPT__

/*
* Generates an alphanumeric key of variable "length"
*/
char* gen_key(int length);


char* xor(char* message, char* key);

/*
* Apply a Xor operation on the "message" using a mask generated with the "key", 
* the lastest mask_xor mask is visible in the mask.txt file
*/
char* mask_xor(char* message, char* key);

/*
* Fetch the last unvisited mask used for the lastest mask_xor function call
*/
char* fetch_mask();


char* cbc_crypt(char* message, char* init_vector, char* key);


char* cbc_uncrypt(char* message, char* init_vector, char* key);

#endif 