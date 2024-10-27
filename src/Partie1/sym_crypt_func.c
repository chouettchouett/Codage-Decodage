#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


#define ALPHA_NUM "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
#define ALPHA_NUM_LENGTH strlen(ALPHA_NUM)-1;

const char* MASK_PATH = "./mask.txt";
#define CBC_MASK_LENGTH 16;


//Note importante : quand mask_crypt ou cbc_crypt est appelé, les masques précédents sont détruits
    // Ainsi, utiliser un mask_xor puis un cbc_crypt effacera les données du mask_xor

char* gen_key(int length){

    char* key = malloc(sizeof(char)*(length+1));

    // remplie chaque case mémoire de key d'un élément aléatoire de ALPHA_NUM
    for(int i=0; i<length; i++)
        key[i] = ALPHA_NUM[rand() % ALPHA_NUM_LENGTH];
    
    return key;
}

int xor(char* message, char* key){
    
    int key_length = strlen(key);
    if(key_length==0){ 
        fprintf(stderr, "xor : key length 0\n");
        return -1;
    }
    int message_length = strlen(message);
    
    for(int i=0; i<message_length; i++)
        message[i] = (char)(message[i]^key[i%key_length]);
    
    return 0;
}


#pragma region FILE_UTILITIES

#define MASK_CHAR_LENGTH 5;
int save_mask(char* mask, int mask_length){
    if(remove(MASK_PATH)){
        perror("remove");
        return -1;
    }
    int fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT);
    if(fd==-1){
        perror("open");
        return -1;
    }

    // Ecriture d'un indicateur de la longueur du mask
    char curMaskLengthChar;
    int curMaskLength = mask_length;
    for(int i=0; i<MASK_CHAR_LENGTH; i++){
        curMaskLengthChar = (mask_length%10)+'0';
        curMaskLength = (int)(curMaskLength/10);
        if(write(fd, &curMaskLengthChar, sizeof(char))==-1){
            perror("write");
            return -1;
        }
    }

    // Ecriture du mask lui même
    if(write(fd, mask, sizeof(char)*mask_length) ==-1){
        perror("write");
        return -1;
    }

    close(fd);
    return 0;
}

char* fetch_mask(){

    // Ouverture du fichier
    int fd = open(MASK_PATH, O_RDONLY);
    if(fd==-1){
        perror("open");
        return NULL;
    }

    // Initialisation
    int i=0
    int mask_size = 0;
    char curNumber;
    if(read(fd, &curNumber, sizeof(char)) ==-1){
        perror("read");
        return NULL;
    }
    
    // Récupération de l'indicateur de taille du mask
    do{
        mask_size = mask_size*10 + (int)(curNumber - '0');
        if(read(fd, &curNumber, sizeof(char)) ==-1){
            perror("read");
            return NULL;
        }
    }while(curNumber!='\n');
    
    // Lecture du mask
    char* mask = malloc(sizeof(char)*(mask_size+1));
    if(read(fd, mask, mask_size) ==-1){
        perror("read");
        return NULL;
    }
    
    // Fermeture du fichier
    if(close(fd) ==-1){
        perror("close");
        return NULL;
    }

    return mask;
}

#pragma endregion FILE_UTILITIES

int mask_xor_crypt(char* message){

    int length = strlen(message);

    // Génération du mask
    char* mask;
    if( (mask=gen_key(length)) ==NULL){
        fprintf("mask_xor_uncrypt : could not fetch mask\n");
        return -1;
    }

    // Cryptage du message en utilisant le mask
    if(xor(message, mask) ==-1){
        fprintf("mask_xor_uncrypt : could not apply xor\n");
        return -1;
    }

    // Stockage du mask
    if(save_mask(mask, length) ==-1){
        fprintf("mask_xor_uncrypt : unable to save mask\n");
        return -1;
    }

    // Déallocation du mask
    free(mask);

    return 0;
}

int mask_xor_uncrypt(char* message){

    // Récupération du dernier mask stocké
    char* mask = fetch_mask();
    if(mask ==NULL){
        fprintf("mask_xor_uncrypt : could not fetch mask\n");
        return -1;
    }

    // Decryptage du message en utilisant le mask
    if(xor(message, mask) ==-1){
        fprintf("mask_xor_uncrypt : could not apply xor\n");
        return -1;
    }

    // Déallocation du mask
    free(mask);

    return 0;
}


#pragma region CBC_UTILITIES

void fill_str(char* message, int startIndex){
    for(int i=startIndex; i<CBC_MASK_LENGTH; i++)
        message[i] = '0';
}

int save_cbc_mask(char* mask, int fd){
    if(write(fd, mask, CBC_MASK_LENGTH) ==-1){
        perror("write");
        return -1;
    }
    return 0;
}

char* fetch_cbc_mask(int fd){
    char* mask = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    if(read(fd, mask, CBC_MASK_LENGTH)==-1){
        perror("read");
        return NULL;
    }
    return mask;
}

int mask_xor_cbc_crypt(char* message, int fd){
    
    int length = strlen(message);

    // Génération du mask
    char* mask = gen_key(length);
    
    // Cryptage du message en utilisant le mask
    if(xor(message, mask)==-1)
        return -1;

    // Stockage du mask
    if(save_cbc_mask(mask, fd)==-1)
        return -1;

    // Déallocation du mask
    free(mask);

    return 0;
}

int mask_xor_cbc_uncrypt(char* message, int fd){
    
    // Récupération du dernier mask stocké
    char* mask = fetch_cbc_mask(fd);
    if(mask == -1)
        return -1;

    // Decryptage du message en utilisant le mask
    if(xor(message, mask) ==NULL)
        return -1;

    // Déallocation du mask
    free(mask);

    return 0;
}

int cbc_crypt_rec(int message_fd, char* vector, int encrypted_fd){

    char message[CBC_MASK_LENGTH];
    // Sachant que l'on garde le même file descriptor, le message lu sera bien le suivant
    int message_length = read(message_fd, message, CBC_MASK_LENGTH);
    if(message_length==-1){
        perror("read");
        return -1;
    }

    if(message_length!=0){ // !condition d'arrêt
        // Remplit le message de '0' si celui-ci n'est pas assez long
        if(message_length<CBC_MASK_LENGTH)
            fill_str(message, message_length);

        // Cryptage
        if(xor(message, vector)==-1) return -1;
        if(mask_xor_cbc_crypt(message)==-1) return -1;
        if(write(encrypted_fd, message, CBC_MASK_LENGTH)==-1){
            perror("write");
            return -1;
        }

        // Décryptage
        if(message_length>=CBC_MASK_LENGTH)
            cbc_crypt_rec(message_fd, message, encrypted_fd);
    }
    return 0;
}

int cbc_uncrypt_rec(int encrypted_fd, char* vector, int message_fd){
    
    char message[CBC_MASK_LENGTH];
    // Sachant que l'on garde le même file descriptor, le message lu sera bien le suivant
    int message_length = read(encrypted_fd, &message, CBC_MASK_LENGTH);
    if(message_length==-1)
        perror("cbc_uncrypt_rec -> read\n");

    if(message_length!=0){ // !condition d'arrêt
        // Verification d'erreur de stockage, car les blocs ont étés remplis
        assert(message_length==CBC_MASK_LENGTH);

        // Stockage temporaire du vecteur pour placer l'appel récursif en fin de programme
        char encrypted_message[CBC_MASK_LENGTH];
        strcpy(encrypted_message, message);

        // Décryptage
        if(mask_xor_cbc_uncrypt(message, M_CBC)==-1) return -1;
        if(xor(message, vector)==-1) return -1;
        if(write(message_fd, message, CBC_MASK_LENGTH) ){
            perror("write");
            return -1;
        }

        // Appel récursif
        cbc_uncrypt_rec(encrypted_fd, encrypted_message, message_fd);
    }
}

#pragma endregion CBC_UTILITIES

void cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath ){

    if(remove(MASK_PATH) ==-1){
        perror("remove");
        fprintf("cbc_crypt : went wrong\n");
        return -1;
    }

    int message_fd;
    int encrypted_fd;
    if( (message_fd=open(message_filepath, O_RDONLY))==-1
        | (encrypted_fd = open(encrypted_filepath, O_WRONLY | O_APPEND | O_CREAT ))==-1 ){
        perror("open");
        fprintf("cbc_crypt : went wrong\n");
        return -1;
    }

    if(cbc_crypt_rec(message_fd, init_vector, encrypted_fd)==-1){
        fprintf("cbc_crypt : went wrong\n");
    }

    if( (close(message_fd)==-1) 
        | (close(encrypted_fd)==-1) ){
        perror("close");
        fprintf("cbc_crypt : went wrong\n");
        return -1;
    }
}

void cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* message_filepath ){
    
    int encrypted_fd;
    int message_fd;
    if( (encrypted_fd = open(encrypted_filepath, O_WRONLY | O_APPEND | O_CREAT ))==-1
        | (message_fd=open(message_filepath, O_RDONLY))==-1 ){
        perror("open");
        fprintf("cbc_uncrypt : went wrong\n");
        return -1;
    }

    if(cbc_uncrypt_rec(encrypted_fd, init_vector, message_fd)==-1){
        fprintf("cbc_uncrypt : went wrong\n");
    }

    if( (close(encrypted_fd)==-1) 
        | (close(message_fd)==-1) ){
        perror("close\n");
        fprintf("cbc_uncrypt : went wrong\n");
        return -1;
    }
}