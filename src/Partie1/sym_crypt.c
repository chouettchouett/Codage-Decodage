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
-o nom_fichier_chiffré -[k clef | f fichier_clé] -m nom_methode [-v vecteur_init]\n"



int HasArgument(char* c){
    for(int i=1; i<argc_; i++){
        if( strcmp(argv_[i], c)==0 ) return i;
    }
    return -1;
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
        int v = HasArgument("-v");// -v vecteur_init
        
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
        set_config(key_path, input_path, output_path);

        if(k!=-1){
            FILE* fd = fopen(key_path, "wb");
            fwrite(argv[k+1], sizeof(char), strlen(argv[k+1]), fd);
            fclose(fd);
        }

        // Appel de la fonction correspondant à la méthode demandée
        if(strcmp(method, "xor")==0){
            printf("xor...\n");
            xor(NULL, NULL);
        }else if(strcmp(method, "cbc-crypt")==0){
            printf("cbc-crypt...\n");
            cbc_crypt(NULL, init_vector, NULL);
        }else if(strcmp(method, "cbc-uncrypt")==0){
            printf("cbc-uncrypt...\n");
            cbc_uncrypt(NULL, init_vector, NULL);
        }else if(strcmp(method, "mask-crypt")==0){
            printf("mask-crypt...\n");
            mask_xor_crypt(NULL);
        }else if(strcmp(method, "mask-uncrypt")==0){
            printf("mask-uncrypt...\n");
            mask_xor_uncrypt(NULL);
        }else {
            fprintf(stderr, "Wrong method name, it should be either one of :\nxor\nmask-crypt\nmask-uncrypt\ncbc-crypt\ncbc-uncrypt\n");
            return 1;
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
        }else if(strcmp("-h", argv[1])==0){
            printf("./sym_crypt -i nom_fichier_message \
-o nom_fichier_chiffré -[k clef | f fichier_clé] -m nom_methode [-v vecteur_init]\n\
\n Valeurs possibles de nom_methode :\n\
xor\nmask-crypt\nmask-uncrypt\ncbc-crypt\ncbc-uncrypt\n\nLors de l'utilisation des méthodes cbc-crypt ou cbc-uncrypt,\n\
il est nécessaire de renseigner un vecteur_init suivant l'argument -v");
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
