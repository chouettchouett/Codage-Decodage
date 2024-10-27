#include "sym_crypt_func.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definition de la macro assert_message
#define assert_message(condition, message) \
    if (!(condition)) { \
        fprintf(stderr, "Echec de l'assertion : %s\nFichier : %s, Ligne : %d\n", message, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

// Fonctions definies dans le code fonctionnel
//char* Xor(char* message, const char* clef);
//int Foncmasq(int m);
//char* Gen_key(int length);
//char* Apply_padding(const char* data, int block_size);
//char* Remove_padding(const char* padded_data);
//void Cbc_crypt(const char* key, const char* input_block, int block_index);
//void Cbc_uncrypt(const char* key, const char* encrypted_block, int block_index);


void Test_Xor() {
    char message[] = "Hello, World!";
    const char* clef = "mysecretkey";

    // Chiffrement du message
    char* chiffre = Xor(message, clef);
    // Dechiffrement du message (reutiliser la meme fonction Propriete)
    char* message_bougee = Xor(chiffre, clef);

    assert_message(strcmp(message_bougee, "Hello, World!") == 0, "Le message chiffre puis dechiffre ne correspond pas au message d'origine.");
}

void Test_Gen_key() {
    char key[11];  // Cle de 10 caracteres + '\0'
    Gen_key(key, 10);
    
    assert_message(strlen(key) == 10, "La longueur de la cle generee n'est pas correcte.");

    // Verifie que tous les caracteres sont alphanumeriques
    for (int i = 0; i < 10; i++) {
        assert_message((key[i] >= 'a' && key[i] <= 'z') ||
                       (key[i] >= 'A' && key[i] <= 'Z') ||
                       (key[i] >= '0' && key[i] <= '9'),
                       "La cle contient des caracteres non alphanumeriques.");
    }
}

void Test_Xor_masque() {
    int m1 = 5;
    int m2 = 10;

    // Valeurs masquees (masque jetable)
    int c1 = Foncmasq(m1);
    int c2 = Foncmasq(m2);

    assert_message(Xor(c1, c2) == Xor(m1, m2), " Dans la comparaison XOR avec masque.");
    assert_message(Xor(Xor(c1, c2), m1) == m2, " Dans le demasquage avec m1.");
    assert_message(Xor(Xor(c1, c2), m2) == m1, " Dans le demasquage avec m2.");
}

void test_padding() {
    char data[] = "hello"; // 5 octets 
    char expected_padded_data[16] = "hello\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B\x0B";  // "hello" + 11 octets de padding (0x0B) [norme PKCS#7]

    char* padded_data = apply_padding(data, 16);

    assert_message(strlen(padded_data) == 16, "La longueur des donnees après padding n'est pas correcte.");
    //assert_message(strcmp(padded_data, expected_padded_data) == 0, "Le padding applique est incorrect.");

    char original_data[16];
    remove_padding(padded_data, original_data);

    assert_message(strcmp(original_data, data) == 0, "Le padding retire ne correspond pas aux donnees d'origine.");

    // Verifier la valeur des octets de padding
    for (int i = 5; i < 16; i++) {
        assert_message(padded_data[i] == 0x0B, "es octets de padding ne contiennent pas la bonne valeur.");
    }
}

void Test_Cbc_crypt() {
    char Bloc1[16] = "Bloc de test....";  // Bloc de 16 octets
    char VI[16] = "InitialisationVI";     // Vecteur d'initialisation de 16 octets
    char key[] = "mysecretkey";           // Cle de test

    char encrypted_bloc1[16];
    char decrypted_bloc1[16];

    Cbc_crypt(key, xor(Bloc1, VI), 0, encrypted_bloc1);

    Cbc_uncrypt(key, encrypted_bloc1, 0, decrypted_bloc1);

    assert_message(strcmp(decrypted_bloc1, Bloc1) == 0, "Le bloc dechiffre ne correspond pas au bloc initial.");
}

// Fonction principale qui execute tous les tests
int main() {
    // Appel des tests
    Test_Xor();
    Test_Xor_masque();
    Test_Gen_key();
    Test_padding();
    Test_Cbc_crypt();

    // Si toutes les assertions passent, afficher ce message
    printf("Tous les tests de la Partie 1 ont ete passes avec succes.\n");

    return 0;
}

//int main(char* argv[], int argc){
    
//}
