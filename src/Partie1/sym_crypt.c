#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "tests_sym_crypt.h"
#include "sym_crypt_utils.h"

int argc_;
char** argv_;

#define WRONG_ARGS "Wrong arguments : ./sym_crypt -i nom_fichier_message \
-o nom_fichier_chiffré -[k clef | f fichier_clé] -m nom_methode [-v nom_fichier_vecteur_init]\n"



int HasArgument(char* c){
    for(int i=1; i<argc_; i++){
        if( strcmp(argv_[i], c)==0 ) return i;
    }
    return -1;
}

// Generalized xor function using the config.txt file
int xor_func(char* key, char* input_path, char* output_path){
    
    FILE* fd_input = fopen(input_path, "rb");
    FILE* fd_output = fopen(output_path, "wb");
    if((fd_input==NULL)
     || fd_output==NULL){
        fprintf(stderr, "Unable to open file\n");
        return -1;
    }
    int key_length = strlen(key);

    char* message;
    while(fread(message, sizeof(char), key_length, fd_input)){
        if(xor(message, key)==-1) return -1;
        fwrite(message, sizeof(char), key_length, fd_output);
    }

    fclose(fd_input);
    fclose(fd_output);
    return 0;
}


#define KEY_PATH "./src/Partie1/key.txt"
#define INIT_VECTOR_PATH "./src/Partie1/init_vector.txt"

int main(int argc, char* argv[]) {
    argc_ = argc;
    argv_ = argv;
    // Initialisation pour rand()
    srand(time(NULL));

    if ((argc==9) || (argc==11)) {
        
        int i = HasArgument("-i");// -i nom_fichier_message
        int o = HasArgument("-o");// -o nom_fichier_chiffré
        int k = HasArgument("-k");// -k clef
        int f = HasArgument("-f");// OU  -f fichier_clé
        int m = HasArgument("-m");// -m nom_methode (parmis xor, cbc-crypt, cbc-uncrypt, mask-crypt, mask-uncrypt)
        int v = HasArgument("-v");// -v nom_fichier_vecteur_init
        
        // Vérifications d'erreurs simples
        if((i==-1)
        || (o==-1)
        || ((k==-1) && (f==-1))
        || (m==-1) ){
            fprintf(stderr, WRONG_ARGS);
            return 1;
        }
        char* method = argv[m+1];
        if( (strcmp(method,"cbc-crypt")==0 || strcmp(method,"cbc-uncrypt")==0) ){
            if((v==-1) 
           || (argc!=11)){
                fprintf(stderr, WRONG_ARGS);
                return 1;
            }
        }else{
            if(argc!=9){
                fprintf(stderr, WRONG_ARGS);
                return 1;
            }
        }

        // Initialisation de la config
        char* key_path = (k==-1)? argv[f+1] : KEY_PATH;
        char* input_path = argv[i+1];
        char* output_path = argv[o+1];
        char* init_vector = (v==-1)? NULL : argv[v+1];
        printf("set_config...\n");
        set_config(key_path, input_path, output_path, init_vector);

        printf("methode choisie : %s", argv[m+1]);
        // Appel de la fonction correspondant à la méthode demandée
        if(strcmp(method, "xor")==0){
            printf("xor...\n");
            if(k!=-1){
                FILE* fd = fopen(key_path, "wb");
                fwrite(argv[k+1], sizeof(char), strlen(argv[k+1]), fd);
                fclose(fd);
            }
            xor_func(NULL, NULL, NULL);
        }if(strcmp(method, "cbc-crypt")==0){
            printf("cbc-crypt...\n");
            // TODO : key filepath ?
            cbc_crypt(NULL, NULL, NULL);
        }if(strcmp(method, "cbc-uncrypt")==0){
            printf("cbc-uncrypt...\n");
            // TODO : key filepath ?
            cbc_uncrypt(NULL, NULL, NULL);
        }if(strcmp(method, "mask-crypt")==0){
            printf("mask-crypt...\n");
            mask_xor_crypt(NULL);
        }if(strcmp(method, "mask-uncrypt")==0){
            printf("mask-uncrypt...\n");
            mask_xor_uncrypt(NULL);
        }
    }
    else if( argc==2){
        if(strcmp("-t", argv[1])==0){
            printf("=== Test XOR simple ===\n");
            test_xor();
            printf("\n=== Test mask XOR crypt ===\n");
            test_mask_xor_crypt();
            printf("\n=== Test CBC crypt ===\n");
            test_cbc_crypt();
            
            printf("\nTous les tests sont réussis.\n");
        }else{
            fprintf(stderr, WRONG_ARGS);
            return 1;
        }
    }else{
        fprintf(stderr, WRONG_ARGS);
        return 1;
    }

    return 0;
}
