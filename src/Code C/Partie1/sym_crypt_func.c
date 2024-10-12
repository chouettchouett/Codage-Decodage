#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

const char* ALPHA_NUM = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int ALPHA_NUM_LENGTH = 62;

const int MASK_SIZE = 16;
const char* MASK_PATH = "mask.txt";

// longueur en octet de l'indicateur de fin de fichier stockant la longueur du dernier mask
const int END_INDICATOR_LENGTH = 4; 


char* gen_key(int length){
    char* key = malloc(sizeof(char)*length);
    for(int i=0; i<length; i++){
        key[i] = ALPHA_NUM[rand() % ALPHA_NUM_LENGTH];
    }
    return key;
}

void xor(char* message, char* key){
    int key_length = strlen(key);
    int message_length = strlen(message);
    
    for(int i=0; i<message_length; i++){
        printf("i=%d\n", i);
        message[i] = (char)(message[i]^key[i%key_length]);
    }
}

void save_mask(char* mask){
    int fd = open(MASK_PATH, O_WRONLY | O_APPEND);

    // Ecriture du mask
    write(fd, mask, strlen(mask));

    // Ajout d'un indicateur de la longueur du dernier mask stocké
    char mask_length[END_INDICATOR_LENGTH];
    sprintf(mask_length, "%d", (int)strlen(mask));
    write(fd, mask_length, END_INDICATOR_LENGTH);
    close(fd);
}

char* fetch_mask(){

    // Ouverture du fichier
    FILE* fstream = fopen(MASK_PATH, "r+");

    // Vérification fichier vide (aucun mask)
    if(EOF)
        return NULL;

    // Récupération de la longueur du dernier mask
    fseek(fstream, END_INDICATOR_LENGTH, SEEK_END);
    char* indicator;
    fgets(indicator, END_INDICATOR_LENGTH, fstream);
    int last_mask_length = atoi(indicator);

    // Lecture du dernier mask
    fseek(fstream, END_INDICATOR_LENGTH+last_mask_length, SEEK_END);
    char* mask;
    fgets(mask, last_mask_length, fstream);

    // Fermeture du fichier
    fclose(fstream);

    return mask;
}

void mask_xor_crypt(char* message){

    // Génération du mask
    char* mask = gen_key(strlen(message));
    
    // Cryptage du message en utilisant le mask
    xor(message, mask);

    // Stockage du mask
    save_mask(mask);
    
    // Déallocation du mask
    free(mask);
}

void mask_xor_uncrypt(char* message){

    // Récupération du dernier mask stocké
    char* mask = fetch_mask();
    
    // Decryptage du message en utilisant le mask
    xor(message, mask);
    
    // Déallocation du mask
    free(mask);
}

void cbc_crypt(char* message, char* init_vector, char* key){
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

void cbc_uncrypt(char* message, char* init_vector, char* key){
    /* TODO
    * - utiliser fetch_mask pour récupérer le dernier mask non réutilisé
    * 
    * - recursive call
    * - garder à l'esprit que les bloc ont été remplis pour le cryptage 
    *   -> pas de cas trop court, seulement le cas 'reste-t-il à décrypter'
    */ 
}