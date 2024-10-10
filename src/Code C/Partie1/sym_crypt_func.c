#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* ALPHA_NUM = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int ALPHA_NUM_LENGTH = 62;

char* gen_key(int length){
    char key[length];
    for(int i=0; i<length; i++){
        key[i] = ALPHA_NUM[rand() % ALPHA_NUM_LENGTH];
    }
    return key;
}

char* xor(char* message, char* key){
    int key_length = strlen(key);
    int message_length = strlen(message);
    
    char crypted_message[message_length];
    for(int i=0; i<message_length; i++){
        crypted_message[i] = message[i]^key[i%key_length];
    }
    return crypted_message;
}

char* mask_xor(char* message){
    int message_length = strlen(message);
    char* mask = gen_key(message_length);
    
    char* crypted_message = xor(message, mask);
    // TODO écrire en mode APPEND le mask utilisé dans mask.txt
    // - (ne pas oublier de close le fichier)

    return crypted_message;
}
char* fetch_mask(){
    
    // TODO lire et renvoyer la dernière ligne de mask.txt
    // - (ne pas oublier de close le fichier)
}

char* cbc_crypt(char* message, char* init_vector, char* key){

    /* TODO
    * - si message insuffisamment long alors remplissage
    *   - sinon si trop long alors découpage
    * 
    * - dans les deux premiers cas :
    *   - utiliser mask_xor, utilisation de l'init_vector et de la key, à voir
    *   - recursive call
    * 
    * - sinon stop
    */
}

char* cbc_uncrypt(char* message, char* init_vector, char* key){

    /* TODO
    * - utiliser fetch_mask pour récupérer le dernier mask non réutilisé
    * 
    * - recursive call
    * - garder à l'esprit que les bloc ont été remplis pour le cryptage 
    *   -> pas de cas trop court, seulement le cas 'reste-t-il à décrypter'
    */ 
}