#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>
#include <math.h>
#include <unistd.h>       
#include <sys/types.h>

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
        key[i] = ALPHA_NUM[abs((int)(rand() % ALPHA_NUM_LENGTH))];
    
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

#define MASK_CHAR_LENGTH 5

int save_length_indicator(int fd, int length, int value){
    char curMaskLengthChar;
    int curMaskLength = value;
    
    for(int i=0; i<length; i++){
        curMaskLengthChar = (curMaskLength%10);
        curMaskLength = (curMaskLength/10);
        if(write(fd, &curMaskLengthChar, sizeof(char))==-1){
            perror("write");
            return -1;
        }
    }
    return 0;
}

int fetch_length_indicator(int fd, int length, int* value){
    char curNumber;
    if(read(fd, &curNumber, sizeof(char)) ==-1){
        perror("read");
        return -1;
    }
    int mask_size = curNumber-'0';
    
    // Récupération de l'indicateur de taille du mask
    for(int i=1; i<length; i++){
        if(read(fd, &curNumber, sizeof(char)) ==-1){
            perror("read");
            return -1;
        }
        mask_size += (int)( (curNumber-'0')*(pow(10, i)) );
    }
    *value = mask_size;
    return 0;
}

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
    if(save_length_indicator(fd, MASK_CHAR_LENGTH, mask_length)==-1) return -1;

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

    // Récupération de la longueur du mask
    int mask_size;
    if(fetch_length_indicator(fd, MASK_CHAR_LENGTH, &mask_size)==-1) return NULL;
    
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
        message[i] = ' ';
}

int save_cbc_mask(char* mask, int mask_fd){
    if(write(mask_fd, mask, sizeof(char)*CBC_MASK_LENGTH) ==-1){
        perror("write");
        return -1;
    }
    return 0;
}

char* fetch_cbc_mask(int mask_fd){
    char* mask = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    // Récupération du mask suivant du même file_descriptor
    if(read(mask_fd, mask, sizeof(char)*CBC_MASK_LENGTH)==-1){
        perror("read");
        return NULL;
    }
    return mask;
}


#pragma endregion CBC_UTILITIES

int cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath ){

    // Réinitialisation des fichiers intermédiaires
    remove(MASK_PATH);
    fclose(fopen(MASK_PATH, "a"));
    chmod(MASK_PATH, S_IRWXU);
    remove(encrypted_filepath);
    fclose(fopen(encrypted_filepath, "a"));
    chmod(encrypted_filepath, S_IRWXU);
    
    // Ouverture des fichiers
    FILE* message_fd = fopen(message_filepath, "r");
    int encrypted_fd;
    int mask_fd;
    if( ((encrypted_fd = open(encrypted_filepath, O_WRONLY | O_APPEND | O_CREAT ))==-1) 
        | ((mask_fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT ))==-1) ){
        perror("open");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    // Initialisation
    struct stat S;
    stat(message_filepath, &S);
    int message_length = S.st_size;
    printf("message_length=%d\n", message_length);
    int read_length = 0;
    
    char message[CBC_MASK_LENGTH+1];
    char encrypted_message[CBC_MASK_LENGTH+1];
    strcpy(message, init_vector);
    fill_str(init_vector, strlen(init_vector));

    int decail;
    char* mask = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    mask[CBC_MASK_LENGTH] = '\n';
    char c = getc(message_fd);

    // Debut traitement

    // TODO : un bloc cbc est parfois traité en trop    
    // TODO : pour la longueur des masks simples, utiliser stat à la place d'un indicateur
    while(read_length<message_length){

        decail=0;
        // mask = gen_key(CBC_MASK_LENGTH);
        

        // remplie chaque case mémoire de key d'un élément aléatoire de ALPHA_NUM
        printf("mask = ");
        
        for(int i=0; i<CBC_MASK_LENGTH; i++){
            mask[i] = ALPHA_NUM[abs((int)(rand() % ALPHA_NUM_LENGTH))];
            printf("%3d ", mask[i]);
        }
        printf("\n");
        if(write(mask_fd, mask, sizeof(char)*CBC_MASK_LENGTH) ==-1){
            perror("write");
            return -1;
        }

        while( (decail<CBC_MASK_LENGTH) & (read_length<message_length) ){
            encrypted_message[decail] = message[decail]^c; // xor vector
            encrypted_message[decail] = encrypted_message[decail]^mask[decail]; // xor mask
            
            message[decail] = encrypted_message[decail];

            c = getc(message_fd);
            decail++;
            read_length++;
        }

        // Cas du dernier bloc cbc
        if(decail<CBC_MASK_LENGTH){
            while(decail<CBC_MASK_LENGTH){
                encrypted_message[decail] = message[decail]^' ';
                encrypted_message[decail] = encrypted_message[decail]^mask[decail];
                
                decail++;
            }
        }
        
        if(write(encrypted_fd, encrypted_message, sizeof(char)*CBC_MASK_LENGTH)==-1){
            perror("write");
            return -1;
        }
        // free(mask);
    }
    // Fin traitement

    // Libération des ressources
    free(mask);
    if( (fclose(message_fd)==-1) 
        | (close(encrypted_fd)==-1)
        | (close(mask_fd)==-1) ){
        perror("close");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    return 0;
}

int cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* uncrypted_filepath ){
    
    // Réinitialisation des fichiers intermédiaires
    remove(uncrypted_filepath);
    fclose(fopen(uncrypted_filepath, "a"));
    chmod(uncrypted_filepath, S_IRWXU);

    // Ouverture des fichiers
    FILE* encrypted_fd = fopen(encrypted_filepath, "r");
    int message_fd;
    int mask_fd;
    if( ((mask_fd=open(MASK_PATH, O_RDONLY))==-1)
      | ((message_fd=open(uncrypted_filepath, O_WRONLY))==-1)){
        perror("open");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    // Initialisation
    struct stat S;
    stat(encrypted_filepath, &S);
    int message_length = S.st_size;
    printf("encrypted_length=%d\n", message_length);
    int read_length = 0;

    char* mask;
    int c = getc(encrypted_fd);
    int decail=0;
    char message[CBC_MASK_LENGTH+1];
    char encrypted_message[CBC_MASK_LENGTH+1];
    strcpy(encrypted_message, init_vector);
    fill_str(encrypted_message, strlen(encrypted_message));

    // Debut traitement
    while(read_length<message_length){
        
        mask = fetch_cbc_mask(mask_fd);
        
        printf("mask = ");
        for(int i=0; i<CBC_MASK_LENGTH; i++){
            printf("%3d ", mask[i]);
        }
        printf("\n");
        while(decail<CBC_MASK_LENGTH){
            message[decail] = c^mask[decail]; // xor mask
            message[decail] = message[decail]^encrypted_message[decail]; // xor vector

            encrypted_message[decail] = c;

            c = getc(encrypted_fd);
            decail++;
            read_length++;
        }
        free(mask);

        decail=0;
        if(write(message_fd, message, sizeof(char)*CBC_MASK_LENGTH)==-1 ){
            perror("write");
            return -1;
        }
    }
    // Fin traitement

    // Libération des ressources
    if( (fclose(encrypted_fd)==-1) 
        | (close(message_fd)==-1) 
        | (close(mask_fd)==-1)){
        perror("close\n");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    return 0;   
}