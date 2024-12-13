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

//TODO : modifier toutes les lectures de clé, elles seront désormais effectuées dans un fichier.
//TODO : utiliser une variable globale pour déterminer le filepath du fichier

//TODO : variables globales des fichiers d' I/O

// TODO : lecture de l'init-vector dans un fichier pour les fonctions cbc;
    // Ajouter une fonction pour ce faire (pour ne pas invalider les tests)

// TODO : mask-crypt & mask-uncrypt => mask (les tests seront innévitablement invalidés)
    // Cependant on a tjrs : appliquer 2 fois mask() => retour au message de base
// TODO : I/O : gen_key, mask

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

int xor(char* message, char* key){
    
    int key_length = strlen(key);
    if(key_length==0){ 
        fprintf(stderr, "xor : key length 0\n");
        return -1;
    }
    int message_length = strlen(message); // INFO : s'arretera éventuellement avant la fin si utilisé sur un message crypté
    
    for(int i=0; i<message_length; i++)
        message[i] = (char)(message[i]^key[i%key_length]);

    return 0;
}

int xor_length(char* message, char* key, int length){
    int key_length = strlen(key);
    if(key_length==0){ 
        fprintf(stderr, "xor : key length 0\n");
        return -1;
    }
    int message_length = length; //
    
    for(int i=0; i<message_length; i++)
        message[i] = (char)(message[i]^key[i%key_length]);

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
    if(fwrite(mask, sizeof(char), mask_length, fd) ==-1){
        perror("write");
        return -1;
    }

    fclose(fd);
    return 0;
}

char* fetch_mask(char* mask_path, int* length){

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
    if(fread(mask, sizeof(char), mask_size, fd) ==-1){
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
    if(message!=NULL){ // appel direct
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
    }else{ // appel par le main, config is set
        
        char* key_path, input_path, output_path;
        get_config(key_path, input_path, output_path, NULL);

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

        mask=malloc(sizeof(char)*length+1);
        mask[length] = '\0';
        
        // Lecture du mask
        if( fread(mask, sizeof(char), length, key_fd)<=0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -5;
        }
        
        // Lecture du message
        if( fread(mask, sizeof(char), length, input_fd)<=0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -6;
        }

        // Cryptage du message en utilisant le mask
        if(xor_length(message, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -7;
        }

        // Ecriture du message crypté
        if(fwrite(message, sizeof(char), length, output_fd)<length){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -8;
        }

        fclose(key_fd);
        fclose(input_fd);
        fclose(output_fd);
    }

    // Déallocation du mask
    free(mask);
    
    return 0;
}

int mask_xor_uncrypt(char* message){

    char* mask;
    int length;

    if(message!=NULL){ // appel direct

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
    }else{ // appel par le main, config is set
        
        char* key_path, input_path, output_path;
        get_config(key_path, input_path, output_path, NULL);

        struct stat key_s, input_s;
        stat(key_path, &key_s);
        stat(input_path, &input_s);
        if(key_s.st_size < input_s.st_size){
            fprintf(stderr, "key size less than message size, impossible for a mask crypt.\n");
            return -4;
        }
        length = input_s.st_size;

        FILE* input_fd = fopen(input_path, "rb");
        FILE* output_fd = fopen(output_path, "wb");

        // Récupération du mask stocké
        if((mask=fetch_mask(key_path, &length)) ==NULL){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -5;
        }
        
        // Lecture du message
        if( fread(mask, sizeof(char), length, input_fd)<=0){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -6;
        }

        // Cryptage du message en utilisant le mask
        if(xor_length(message, mask, length) ==-1){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -7;
        }

        // Ecriture du message crypté
        if(fwrite(message, sizeof(char), length, output_fd)<length){
            fprintf(stderr, "mask_xor_uncrypt : went wrong\n");
            return -8;
        }

        fclose(input_fd);
        fclose(output_fd);
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
    FILE* message_fd = fopen(message_filepath, "rb");
    FILE* encrypted_fd = fopen(encrypted_filepath, "wb");
    int mask_fd;
    if( (message_fd==NULL) | (encrypted_fd==NULL)
        | ((mask_fd = open(MASK_PATH, O_WRONLY | O_APPEND | O_CREAT ))==-1) ){
        perror("open");
        fprintf(stderr, "cbc_crypt : went wrong\n");
        return -1;
    }

    // Initialisation
    struct stat S;
    stat(message_filepath, &S);
    int message_length = S.st_size;
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
        for(int i=0; i<CBC_MASK_LENGTH; i++){
            mask[i] = ALPHA_NUM[abs((int)(rand() % ALPHA_NUM_LENGTH))];
        }

        if(write(mask_fd, mask, sizeof(char)*CBC_MASK_LENGTH) ==-1){
            perror("write");
            return -1;
        }

        while( (decail<CBC_MASK_LENGTH) & (read_length<message_length) ){
            //while((c=getc(message_fd))==13);
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
        
        if(fwrite(encrypted_message, sizeof(char), CBC_MASK_LENGTH, encrypted_fd)!=CBC_MASK_LENGTH){
            perror("write");
            return -1;
        }
        // free(mask);
    }
    // Fin traitement

    // Libération des ressources
    free(mask);
    if( (fclose(message_fd)==-1) 
        | (fclose(encrypted_fd)==-1)
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
    FILE* encrypted_fd = fopen(encrypted_filepath, "rb");
    FILE* message_fd = fopen(uncrypted_filepath, "wb");
    int mask_fd;
    if( ((mask_fd=open(MASK_PATH, O_RDONLY))==-1)
      | (message_fd==NULL) | (encrypted_fd==NULL)){
        perror("open");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    // Initialisation
    struct stat S;
    stat(MASK_PATH, &S);
    int message_length = S.st_size;
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
        if(fwrite(message, sizeof(char), CBC_MASK_LENGTH, message_fd)!=CBC_MASK_LENGTH ){
            perror("write");
            return -1;
        }
    }
    // Fin traitement

    // Libération des ressources
    if( (fclose(encrypted_fd)==-1) 
        | (fclose(message_fd)==-1) 
        | (close(mask_fd)==-1)){
        perror("close\n");
        fprintf(stderr, "cbc_uncrypt : went wrong\n");
        return -1;
    }

    return 0;   
}