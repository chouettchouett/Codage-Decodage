#ifndef __FUNC_SYM_CRYPT_Test__
#define __FUNC_SYM_CRYPT_Test__
    
void test_xor();

void test_mask_xor_crypt();

void unpadding(char *message);

void afficher_et_unppad(FILE *f_dec, char *buffer);

void test_cbc_crypt();

#endif
