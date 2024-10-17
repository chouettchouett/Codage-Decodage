#ifndef __FUNC_SYM_CRYPT__
#define __FUNC_SYM_CRYPT__

static const int M_CRYPT = 1;
static const int M_DECRYPT = 2;

/*
* Generates an alphanumeric key of variable "length"
*/
char* gen_key(int length);


void xor(char* message, char* key);

/*
* Valeurs possibles de "operation" :
*   MX_CRYPT : créer puis stocke un mask après l'avoir utiliser pour crypter "message"
*   MX_DECRYPT : utilise le dernier mask stocké pour decrypter "message"
*/
void mask_xor(char* message, int operation);

void cbc_crypt(char* message, char* init_vector, char* key);


void cbc_uncrypt(char* message, char* init_vector, char* key);

#endif