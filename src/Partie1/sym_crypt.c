#include "sym_crypt_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "tests_sym_crypt.h"

int argc_;
char* argv_[];

int HasArgument(char* c){
    for(int i=1; i<argc_; i++){
        if( strcmp(argv_[i], c)==0 ) return i;
    }
    return -1;
}

#define message_max_length 100000
int GetMessageFromFilepath(char* message, char* filepath){
    FILE* fd = fopen(filepath, "rb");

    if(fd==-1){
        fprintf("message_filepath open");
        return -1;
    }
    malloc(sizeof(char)*message_max_length);
    while(fread(&message[i], sizeof(char), 2048, fd)>0)
        i+=2048*sizeof(char);

    fclose(fd);
}

#define WrongArgsString "Wrong arguments : ./sym_crypt -i nom_fichier_message\
             -o nom_fichier_chiffré -[k clef,f fichier_clé] -m nom_methode -v nom_fichier_vecteur_init"


int xor_func(char* input_path, char* key, char* output_path){
    FILE* fd_input = fopen(input_path, "rb");
    FILE* fd_output = fopen(output_path, "wb");
    if((fd_input==NULL)
     || fd_output==NULL){
        fprintf(stderr, "Unable to open file"); perror("");
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
}

int main(int argc, char* argv[]) {
    argc_ = argc;
    argv_ = argv;
    // Initialisation pour rand()
    srand(time(NULL));

    if (argc == 6) {
        
        int i = HasArgument("-i");// -i nom_fichier_message
        int o = HasArgument("-o");// -o nom_fichier_chiffré
        int k = HasArgument("-k");// -k clef
        int f = HasArgument("-f");// OU  -f fichier_clé
        int m = HasArgument("-m");// -m nom_methode (parmis xor, cbc-crypt, cbc-uncrypt, mask-crypt, mask-uncrypt)
        int v = HasArgument("-v");// -v nom_fichier_vecteur_init
        
        if((i==-1)
        || (o==-1)
        || ((k==-1) && (f==-1))
        || (m==-1) ){
            fprintf(stderr, WrongArgsString);
            return 1;
        }

        char* method = argv[m];
        if(argv[m]!="cbc-crypt" && argv[m]!="cbc-uncrypt"){
            if(argv[m]=="mask-crypt" || argv[m]=="mask-uncrypt") GetMessageFromFilepath(message, argv[i]);
        }else{
            if (v==-1){
                fprintf(stderr, WrongArgsString);
                return 1;
            }
        }

        switch(method){
            case "xor":
                if(f==-1) xor_func_withkey(argv[i], argv[k], argv[o]);
                // TODO : else xor_func_withpath ?
            case "cbc-crypt":
                // TODO : key filepath ?
                cbc_crypt(argv[i], init_vector, argv[o]);
            case "cbc-uncrypt":
                // TODO : key filepath ?
                cbc_uncrypt(argv[i], init_vector, argv[o] );
            case "mask-crypt":
                // TODO : key filepath ? I/O
                mask_xor_crypt(message);
            case "mask-uncrypt":
                // TODO : key filepath ? I/O
                mask_xor_uncrypt(message);
        }
    }else if( argc==2){
        if(HasArgument("-t")){
            printf("=== Test XOR simple ===\n");
            test_xor();
            printf("\n=== Test mask XOR crypt ===\n");
            test_mask_xor_crypt();
            printf("\n=== Test CBC crypt ===\n");
            test_cbc_crypt();
            
            printf("\nTous les tests sont réussis.\n");
        }else{
            fprintf(stderr, WrongArgsString);
        }
    }

    return 0;
}
