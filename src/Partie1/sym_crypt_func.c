#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>

#define ALPHA_NUM "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHA_NUM_LENGTH strlen(ALPHA_NUM)-1

const char* MASK_PATH = "./mask.txt";
#define CBC_MASK_LENGTH 16


//Note importante : quand mask_crypt ou cbc_crypt est appelé, les masques précédents sont détruits
    // Ainsi, utiliser un mask_xor puis un cbc_crypt effacera les données du mask_xor

char* gen_key(int length){

    char* key = malloc(sizeof(char)*(length+1));

    // remplie chaque case mémoire de key d'un élément aléatoire de ALPHA_NUM
    for(int i=0; i<length; i++)
        key[i] = ALPHA_NUM[rand() % ALPHA_NUM_LENGTH] + '0';
    
    return key;
}

int xor(char* message, char* key){
    
    int key_length = strlen(key);
    if(key_length==0){ 
        fprintf(stderr, "xor : key length 0\n");
        return -1;
    }
    int message_length = strlen(message);
    
    for(int i=0; i<message_length; i++){
        printf("char keycode = %d\n", key[i%key_length]);
        message[i] = (char)(message[i]^key[i%key_length]);
    }

    return 0;
}


#pragma region FILE_UTILITIES

#define MASK_CHAR_LENGTH 5
int save_mask(char* mask, int mask_length){

    remove(MASK_PATH);
    fclose(fopen(MASK_PATH, "a"));
    chmod(MASK_PATH, S_IRWXU);

    int fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT);
    if(fd==-1){
        perror("open");
        return -1;
    }

    // Ecriture d'un indicateur de la longueur du mask
    char curMaskLengthChar;
    int curMaskLength = mask_length;
    
    for(int i=0; i<MASK_CHAR_LENGTH; i++){
        curMaskLengthChar = (curMaskLength%10)+'0';
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

    printf("mask = %s\n", mask);
    close(fd);
    return 0;
}

char* fetch_mask(){

    // Ouverture du fichier
    int fd = open(MASK_PATH, O_RDWR);
    if(fd==-1){
        perror("open");
        return NULL;
    }

    // Initialisation
    int i=0;
    int mask_size = 0;
    char curNumber;
    if(read(fd, &curNumber, sizeof(char)) ==-1){
        perror("read");
        return NULL;
    }
    
    // Récupération de l'indicateur de taille du mask
    do{
        mask_size += (int)(( curNumber*(pow(10, 10*i)) ) - '0');
        if(read(fd, &curNumber, sizeof(char)) ==-1){
            perror("read");
            return NULL;
        }
        i++;
    }while(i<5);
    
    // Lecture du mask
    char* mask = malloc(sizeof(char)*(mask_size+1));
    if(read(fd, mask, sizeof(char)*mask_size) ==-1){
        perror("read");
        return NULL;
    }
    
    // Fermeture du fichier
    if(close(fd) ==-1){
        perror("close");
        return NULL;
    }
    printf("mask = %s\n\n", mask);

    char text[mask_size+MASK_CHAR_LENGTH+1];
    read(open(MASK_PATH, O_RDONLY), &text, sizeof(char)*mask_size+MASK_CHAR_LENGTH);
    for(int i=0; i<mask_size+MASK_CHAR_LENGTH; i++)
        printf("verify read char = %d\n", text[i]);

    return mask;
}

#pragma endregion FILE_UTILITIES

int mask_xor_crypt(char* message){

    int length = strlen(message);

    // Génération du mask
    char* mask;
    if( (mask=gen_key(length)) ==NULL){
        fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
        return -1;
    }

    // Cryptage du message en utilisant le mask
    if(xor(message, mask) ==-1){
        fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
        return -1;
    }

    // Stockage du mask
    if(save_mask(mask, length) ==-1){
        fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
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
        fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
        return -1;
    }

    // Decryptage du message en utilisant le mask
    if(xor(message, mask) ==-1){
        fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
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

int save_cbc_mask(char* mask, int mask_fd){
    if(write(mask_fd, mask, CBC_MASK_LENGTH) ==-1){
        perror("write");
        return -1;
    }
    return 0;
}

char* fetch_cbc_mask(int mask_fd){
    char* mask = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    // Récupération du mask suivant du même file_descriptor
    if(read(mask_fd, mask, CBC_MASK_LENGTH)==-1){
        perror("read");
        return NULL;
    }
    return mask;
}

int mask_xor_cbc_crypt(char* message, int mask_fd){
    
    int length = strlen(message);

    // Génération du mask
    char* mask = gen_key(length);
    
    // Cryptage du message en utilisant le mask
    if(xor(message, mask)==-1)
        return -1;

    // Stockage du mask
    if(save_cbc_mask(mask, mask_fd)==-1)
        return -1;

    // Déallocation du mask
    free(mask);

    return 0;
}

int mask_xor_cbc_uncrypt(char* message, int mask_fd){
    
    // Récupération du dernier mask stocké
    char* mask = fetch_cbc_mask(mask_fd);
    if(mask == NULL)
        return -1;

    // Decryptage du message en utilisant le mask
    if(xor(message, mask) ==-1)
        return -1;

    // Déallocation du mask
    free(mask);

    return 0;
}

int cbc_crypt_rec(int message_fd, char* vector, int encrypted_fd, int mask_fd){

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
        if(mask_xor_cbc_crypt(message, mask_fd)==-1) return -1;
        if(write(encrypted_fd, message, CBC_MASK_LENGTH)==-1){
            perror("write");
            return -1;
        }

        // Décryptage
        if(message_length>=CBC_MASK_LENGTH)
            cbc_crypt_rec(message_fd, message, encrypted_fd, mask_fd);
    }
    return 0;
}

int cbc_uncrypt_rec(int encrypted_fd, char* vector, int message_fd, int mask_fd){
    
    char message[CBC_MASK_LENGTH];
    // Lecture de la suite du message sur le même file_descriptor
    int message_length = read(encrypted_fd, &message, CBC_MASK_LENGTH);
    if(message_length==-1)
        return -1;

    if(message_length!=0){ // !condition d'arrêt
        // Verification d'erreur de stockage, car les blocs ont étés remplis
        assert(message_length==CBC_MASK_LENGTH);

        // Stockage temporaire du vecteur pour placer l'appel récursif en fin de programme
        char encrypted_message[CBC_MASK_LENGTH];
        strcpy(encrypted_message, message);

        // Décryptage
        if(mask_xor_cbc_uncrypt(message, mask_fd)==-1) return -1;
        if(xor(message, vector)==-1) return -1;
        if(write(message_fd, message, CBC_MASK_LENGTH) ){
            perror("write");
            return -1;
        }

        // Appel récursif
        cbc_uncrypt_rec(encrypted_fd, encrypted_message, message_fd, mask_fd);
    }
    return 0;
}

#pragma endregion CBC_UTILITIES

int cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath ){

    remove(MASK_PATH);
    fclose(fopen(MASK_PATH, "a"));
    chmod(MASK_PATH, S_IRWXU);

    int message_fd;
    int encrypted_fd;
    int mask_fd;
    if( ((message_fd=open(message_filepath, O_RDWR))==-1)
        | ((encrypted_fd = open(encrypted_filepath, O_WRONLY | O_APPEND | O_CREAT ))==-1) 
        | ((mask_fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT ))==-1) ){
        perror("open");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    if(cbc_crypt_rec(message_fd, init_vector, encrypted_fd, mask_fd)==-1){
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    if( (close(message_fd)==-1) 
        | (close(encrypted_fd)==-1) ){
        perror("close");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    return 0;
}

int cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* message_filepath ){
    
    int encrypted_fd;
    int message_fd;
    int mask_fd;
    if( ((encrypted_fd = open(encrypted_filepath, O_WRONLY | O_APPEND | O_CREAT ))==-1)
        | ((message_fd=open(message_filepath, O_RDONLY))==-1) 
        | ((mask_fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT ))==-1) ){
        perror("open");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    if(cbc_uncrypt_rec(encrypted_fd, init_vector, message_fd, mask_fd)==-1){
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    if( (close(encrypted_fd)==-1) 
        | (close(message_fd)==-1) ){
        perror("close\n");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    return 0;
}