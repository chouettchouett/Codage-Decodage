#include "sym_crypt_func.h"
#include "sym_crypt_utils.h"
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

const char* MASK_PATH = "./src/Partie1/mask.txt";
#define CBC_MASK_LENGTH 16

//Note importante : quand mask_crypt ou cbc_crypt est appelé, les masques précédents sont détruits
    // Ainsi, utiliser un mask_xor puis un cbc_crypt effacera les données du mask_xor

char* gen_key(int length){

    char* key = malloc(sizeof(char)*(length+1));
    int i;

    // remplie chaque case mémoire de key d'un élément aléatoire de ALPHA_NUM
    for(i = 0; i < length; i++)
        key[i] = ALPHA_NUM[abs((int)(rand() % ALPHA_NUM_LENGTH))];
    
    key[i] = '\0';
    
    return key;
}

// Is limited to an alphanumeric key (no character \0)
int xor_length(char* message, char* key, int length){
    int key_length = strlen(key);
    if(key_length==0){ 
        fprintf(stderr, "xor : key length 0\n");
        return -1;
    }
    
    for(int i=0; i<length; i++)
        message[i] = (char)(message[i]^key[i%(key_length+1)]);

    return 0;
}

int xor(char* message, char* key){
    if((message!=NULL) && (key!=NULL)){ // Utilisation normale
        int key_length = strlen(key);
        if(key_length==0){ 
            fprintf(stderr, "xor : key length 0\n");
            return -1;
        }
        int message_length = strlen(message);
        // INFO : s'arretera éventuellement avant la fin si utilisé sur un message crypté
            // (contenant éventuellement \0)
        
        for(int i=0; i<message_length; i++)
            message[i] = (char)(message[i]^key[i%key_length]);

        return 0;
    }else{ // Lecture dans le fichier config
        char* key_path = malloc(sizeof(char)*100);
        char* input_path = malloc(sizeof(char)*100);
        char* output_path = malloc(sizeof(char)*100);

        get_config(key_path, input_path, output_path);

        struct stat key_s, input_s;
        stat(key_path, &key_s);
        stat(input_path, &input_s);
        int key_length = key_s.st_size;
        int message_length = input_s.st_size;

        FILE* key_fd = fopen(key_path, "rb");
        FILE* input_fd = fopen(input_path, "rb");
        FILE* output_fd = fopen(output_path, "wb");

        if((key_fd==NULL)
        || (input_fd==NULL)
        || (output_fd==NULL)){
            perror("xor fopen :");
            fprintf(stderr, "Unable to open config files\nxor : went wrong\n");
            return 1;
        }

        char* m = malloc(sizeof(char)*(message_length+1));
        if( fread(m, sizeof(char), message_length, input_fd)<=0){
            fprintf(stderr, "xor : went wrong\n");
            return 1;
        }
        char* k = malloc(sizeof(char)*(key_length+1));
        if( fread(k, sizeof(char), key_length, key_fd)<=0){
            fprintf(stderr, "xor : went wrong\n");
            return 1;
        }
        k[key_length]='\0';

        if(xor_length(m, k, message_length)!=0){
            fprintf(stderr, "xor : went wrong\n");
            return 1;
        }

        if( fwrite(m, sizeof(char), message_length, output_fd)<=0){
            fprintf(stderr, "xor : went wrong\n");
            return 1;
        }

        fclose(key_fd);
        fclose(input_fd);
        fclose(output_fd);

        free(key_path);
        free(input_path);
        free(output_path);
    }
    return 0;
}

#pragma region FILE_UTILITIES

int save_mask(char* mask, int mask_length){

    FILE* fd = fopen(MASK_PATH, "wb");
    if(fd==NULL){
        perror("open");
        return -1;
    }

    // Ecriture du mask
    if(fwrite(mask, sizeof(char), mask_length, fd) ==0){
        perror("write");
        return -1;
    }

    fclose(fd);
    return 0;
}

char* fetch_mask(const char* mask_path, int* length){

    // Ouverture du fichier
    FILE* fd = fopen(mask_path, "rb");
    if(fd==NULL){
        perror("open");
        return NULL;
    }

    // Récupération de la longueur du mask
    struct stat S;
    stat(mask_path, &S);
    int mask_size = S.st_size;
    *length = S.st_size;

    // Lecture du mask
    char* mask = malloc(sizeof(char)*(mask_size+1));
    mask[mask_size*sizeof(char)]='\0';
    if(fread(mask, sizeof(char), mask_size, fd)==0){
        perror("read");
        return NULL;
    }

    // Fermeture du fichier
    if(fclose(fd) ==-1){
        perror("close");
        return NULL;
    }

    return mask;
}



#pragma endregion FILE_UTILITIES

int mask_xor_crypt(char* message){

    int length;
    char* mask;
    if(message!=NULL){ // Utilisation normal
        length = strlen(message);

        // Génération du mask
        if( (mask=gen_key(length)) ==NULL){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -1;
        }

        // Cryptage du message en utilisant le mask
        if(xor_length(message, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -2;
        }
        
        // Stockage du mask
        if(save_mask(mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -3;
        }
    }else{ // Lecture dans le fichier config
        char* key_path = malloc(sizeof(char)*100);
        char* input_path = malloc(sizeof(char)*100);
        char* output_path = malloc(sizeof(char)*100);

        get_config(key_path, input_path, output_path);

        struct stat key_s, input_s;
        stat(key_path, &key_s);
        stat(input_path, &input_s);
        if(key_s.st_size < input_s.st_size){
            fprintf(stderr, "key size less than message size, impossible for a mask crypt.\n");
            return -4;
        }
        length = input_s.st_size;

        FILE* key_fd = fopen(key_path, "rb");
        FILE* input_fd = fopen(input_path, "rb");
        FILE* output_fd = fopen(output_path, "wb");
        
        if((key_fd==NULL)
        || (input_fd==NULL)
        || (output_fd==NULL)){
            perror("mask-crypt fopen :");
            return 1;
        }

        mask=malloc(sizeof(char)*length+1);
        mask[length] = '\0';
        
        // Lecture du mask
        if( fread(mask, sizeof(char), length, key_fd)==0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -5;
        }

        char* m = malloc(sizeof(char)*(length+1));
        // Lecture du message
        if( fread(m, sizeof(char), length, input_fd)==0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -6;
        }

        // Cryptage du message en utilisant le mask
        if(xor_length(m, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -7;
        }

        // Ecriture du message crypté
        if(fwrite(m, sizeof(char), length, output_fd)<(size_t)length){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -8;
        }

        fclose(key_fd);
        fclose(input_fd);
        fclose(output_fd);

        // Stockage du mask
        if(save_mask(mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -3;
        }
        free(m);
    }

    // Déallocation du mask
    free(mask);
    
    return 0;
}

int mask_xor_uncrypt(char* message){

    char* mask;
    int length;

    if(message!=NULL){ // Utilisation normale

        // Récupération du dernier mask stocké
        mask = fetch_mask(MASK_PATH, &length);
        if(mask ==NULL){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -1;
        }

        // Decryptage du message en utilisant le mask
        if(xor_length(message, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -2;
        }

        length = strlen(message);
        
        // Stockage du mask
        if(save_mask(mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -3;
        }
    }else{ // Lecture dans le fichier config
        
        char* input_path = malloc(sizeof(char)*100);
        char* output_path = malloc(sizeof(char)*100);

        get_config(NULL, input_path, output_path);

        struct stat key_s, input_s;
        stat(MASK_PATH, &key_s);
        stat(input_path, &input_s);
        if(key_s.st_size < input_s.st_size){
            fprintf(stderr, "key size less than message size, impossible for a mask crypt.\n");
            return -4;
        }
        length = input_s.st_size;

        FILE* input_fd = fopen(input_path, "rb");
        FILE* output_fd = fopen(output_path, "wb");

        // Récupération du mask stocké
        if((mask=fetch_mask(MASK_PATH, &length)) ==NULL){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -5;
        }
        
        char* m = malloc(sizeof(char)*(length+1));
        // Lecture du message
        if( fread(m, sizeof(char), length, input_fd)==0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -6;
        }

        // Cryptage du message en utilisant le mask
        if(xor_length(m, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -7;
        }

        // Ecriture du message crypté
        if(fwrite(m, sizeof(char), length, output_fd)<(size_t)length){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -8;
        }

        fclose(input_fd);
        fclose(output_fd);

        free(input_path);
        free(output_path);

        free(m);
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

int save_cbc_mask(char* mask, FILE* mask_fd){
    if(fwrite(mask, sizeof(char), CBC_MASK_LENGTH, mask_fd) ==0){
        perror("write");
        return -1;
    }
    return 0;
}

char* fetch_cbc_mask(FILE* mask_fd){
    char* mask = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    // Récupération du mask suivant du même file_descriptor
    if(fread(mask, sizeof(char), CBC_MASK_LENGTH, mask_fd)==0){
        perror("read");
        return NULL;
    }
    return mask;
}

int cbc_crypt_rec(char* message, FILE* message_fd, char* vector, FILE* encrypted_fd, FILE* mask_fd, FILE* key_fd){

    // Lecture de la suite du message
    int message_length = fread(message, sizeof(char), CBC_MASK_LENGTH, message_fd);
    if(message_length==0) return 0;
    if(message_length<CBC_MASK_LENGTH) fill_str(message, message_length);
    
    // Utilisation du vector
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        message[i] ^= vector[i];

    // Lecture de la suite du mask (ou génération de celui-ci)
    char* key;
    if(key_fd==NULL){// Generated key
        key = gen_key(CBC_MASK_LENGTH);
    }else {// Reading a key file
        key = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
        int key_length = fread(key, sizeof(char), CBC_MASK_LENGTH, key_fd);
        if(message_length>key_length){
            fprintf(stderr, "cbc_crypt_rec get mask went wrong\n");
            return 1;
        }
    }
    
    // Utilisation du mask
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        message[i] ^= key[i];
    
    // Sauvegarde du mask
    save_cbc_mask(key, mask_fd);
    free(key);

    // Ecriture du message crypté
    if(fwrite(message, sizeof(char), CBC_MASK_LENGTH, encrypted_fd)==0){
        fprintf(stderr, "cbc_crypt_rec write message went wrong\n");
        return 1;
    }

    for(int i=0; i<CBC_MASK_LENGTH; i++)
        vector[i]=message[i];

    if(message_length!=16) return 0;
    return cbc_crypt_rec(message, message_fd, vector, encrypted_fd, mask_fd, key_fd);
}

int cbc_uncrypt_rec(char* message, FILE* encrypted_fd, char* vector, FILE* recovered_fd, FILE* mask_fd, FILE* key_fd){
    
    int message_length = fread(message, sizeof(char), CBC_MASK_LENGTH, encrypted_fd);
    if(message_length==0) return 0;
    char* intermed = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        intermed[i]=message[i];

    // Récupération du mask
    char* key;
    if(key_fd==NULL) key = fetch_cbc_mask(mask_fd);
    else key = fetch_cbc_mask(key_fd);

    // Utilisation du mask
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        message[i] ^= key[i];
    free(key);

    // Utilisation du vector
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        message[i] ^= vector[i];

    // Ecriture du message décrypté
    if(fwrite(message, sizeof(char), CBC_MASK_LENGTH, recovered_fd)==0){
        fprintf(stderr, "cbc_crypt_rec write message went wrong\n");
        return 1;
    }
    
    for(int i=0; i<CBC_MASK_LENGTH; i++)
        vector[i]=intermed[i];
    free(intermed);
    if(message_length!=16) return 0;
    return cbc_uncrypt_rec(message, encrypted_fd, vector, recovered_fd, mask_fd, key_fd);
}

#pragma endregion CBC_UTILITIES

int cbc_crypt(char* message_filepath, char* init_vector, char* encrypted_filepath ){

    // Initialisation
    FILE* message_fd;
    FILE* encrypted_fd;
    FILE* mask_fd;
    FILE* key_fd = NULL;
    char* vector = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    char* message = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    strncpy(vector, init_vector, CBC_MASK_LENGTH);
    
    // Ouverture des fichiers
    if((message_filepath==NULL) && (encrypted_filepath==NULL)){ // Récupération de la config
        message_filepath = malloc(sizeof(char)*100);
        encrypted_filepath = malloc(sizeof(char)*100);
        char* key_filepath = malloc(sizeof(char)*100);
        get_config(key_filepath, message_filepath, encrypted_filepath);
        
        message_fd = fopen(message_filepath, "rb");
        encrypted_fd = fopen(encrypted_filepath, "wb");
        mask_fd = fopen(MASK_PATH, "wb");
        
        if((key_fd = fopen(key_filepath, "rb")) == NULL){
            fprintf(stderr, "cbc_crypt could not open config key file");
            return 1;
        }
        free(message_filepath);
        free(encrypted_filepath);
        free(key_filepath);
    }else{ // Ouverture normale des fichiers
        message_fd = fopen(message_filepath, "rb");
        encrypted_fd = fopen(encrypted_filepath, "wb");
        mask_fd = fopen(MASK_PATH, "wb");
    }

    if( (message_fd==NULL) 
        | (encrypted_fd==NULL)
        | (mask_fd==NULL)){
        perror("open");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }
    
    printf("dd\n");
    cbc_crypt_rec(message, message_fd, vector, encrypted_fd, mask_fd, key_fd);
    printf("ff\n");

    // Libération des ressources
    if( (fclose(message_fd)==-1) 
        | (fclose(encrypted_fd)==-1)
        | (fclose(mask_fd)==-1) ){
        perror("close");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }
    if(!(key_fd==NULL)) fclose(key_fd);

    free(vector);
    free(message);

    return 0;
}

int cbc_uncrypt(char* encrypted_filepath, char* init_vector, char* uncrypted_filepath ){
    
    // Initialisation
    FILE* encrypted_fd;
    FILE* uncrypted_fd;
    FILE* mask_fd;
    FILE* key_fd = NULL;
    char* vector = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    char* message = malloc(sizeof(char)*(CBC_MASK_LENGTH+1));
    strncpy(vector, init_vector, CBC_MASK_LENGTH);

    // Ouverture des fichiers
    if((encrypted_filepath==NULL) && (uncrypted_filepath==NULL)){ // Récupération de la config
        encrypted_filepath = malloc(sizeof(char)*100);
        uncrypted_filepath = malloc(sizeof(char)*100);
        char* key_filepath = malloc(sizeof(char)*100);
        get_config(key_filepath, encrypted_filepath, uncrypted_filepath);

        encrypted_fd = fopen(encrypted_filepath, "rb");
        uncrypted_fd = fopen(uncrypted_filepath, "wb");
        mask_fd = fopen(MASK_PATH, "rb");
        
        if((key_fd = fopen(key_filepath, "rb")) == NULL){
            fprintf(stderr, "cbc_crypt could not open config key file");
            return 1;
        }
        free(encrypted_filepath);
        free(uncrypted_filepath);
        free(key_filepath);
    }else{ // Ouverture normale des fichiers
        encrypted_fd = fopen(encrypted_filepath, "rb");
        uncrypted_fd = fopen(uncrypted_filepath, "wb");
        mask_fd = fopen(MASK_PATH, "rb");
    }

    if( (encrypted_fd==NULL) 
        | (uncrypted_fd==NULL)
        | (mask_fd==NULL)){
        perror("open");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    printf("dd\n");
    cbc_uncrypt_rec(message, encrypted_fd, vector, uncrypted_fd, mask_fd, key_fd);
    printf("ff\n");

    // Libération des ressources
    if( (fclose(encrypted_fd)==-1) 
        | (fclose(uncrypted_fd)==-1)
        | (fclose(mask_fd)==-1) ){
        perror("close");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }
    if(!(key_fd==NULL)) fclose(key_fd);

    free(vector);
    free(message);

    return 0;   
}