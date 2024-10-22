#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


const char* ALPHA_NUM = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const int ALPHA_NUM_LENGTH = 62;

const char* MASK_PATH = "mask.txt";
const int MAX_MASK_LENGTH = 256; 


char* gen_key(int length){
    char* key = malloc(sizeof(char)*length);
    for(int i=0; i<length; i++)
        key[i] = ALPHA_NUM[rand() % ALPHA_NUM_LENGTH];
    
    return key;
}

void xor(char* message, char* key){
    int key_length = strlen(key);
    if(key_length==0) fprintf(stderr, "xor : key length 0\n");
    int message_length = strlen(message);
    
    for(int i=0; i<message_length; i++)
        message[i] = (char)(message[i]^key[i%key_length]);
}

#pragma region MASK_UTILITIES

void save_mask(char* mask){
    int fd = open(MASK_PATH, O_WRONLY | O_APPEND);

    // Ecriture du mask
    write(fd, mask, strlen(mask));

    close(fd);
}

char* fetch_mask(){

    // Ouverture du fichier
    FILE* fstream = fopen(MASK_PATH, "r+");

    // Lecture du dernier mask
    char* mask;
    fscanf(fstream, "%s", mask);

    // Fermeture du fichier
    fclose(fstream);

    return mask;
}

void mask_xor_crypt(char* message){

    // Génération du mask
    char* mask = gen_key(strlen(message));
    printf("\nmask genere...\n");
    // Cryptage du message en utilisant le mask
    xor(message, mask);
    printf("xor applique...\n");
    // Stockage du mask
    save_mask(mask);
    printf("mask stocke : %s ... \n", mask);
    // Déallocation du mask
    free(mask);
    printf("mask_xor_crypt SUCCESS \n\n");
}

void mask_xor_decrypt(char* message){

    // Récupération du dernier mask stocké
    char* mask = fetch_mask();
    printf("mask recupere : %s ...\n", mask);
    // Decryptage du message en utilisant le mask
    xor(message, mask);
    printf("xor applique...\n");
    // Déallocation du mask
    free(mask);
    printf("mask_xor_decrypt SUCCESS \n\n");
}

#pragma endregion MASK

void mask_xor(char* message, int operation){

    if( operation==M_CRYPT ){
        mask_xor_crypt(message);
    }
    else if( operation==M_DECRYPT ){
        mask_xor_decrypt(message);
    }
    else{
        fprintf(stderr, "mask_xor : Wrong mask operator.");
    }
    
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