#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define KEY_STR "KEY_PATH "
#define INPUT_STR "INPUT_PATH "
#define OUTPUT_STR "OUTPUT_PATH "
#define INIT_VECTOR_STR "INIT_VECTOR_PATH "

#define KEY_STR_LENGTH strlen(KEY_STR)
#define INPUT_STR_LENGTH strlen(INPUT_STR)
#define OUTPUT_STR_LENGTH strlen(OUTPUT_STR)
#define INIT_VECTOR_STR_LENGTH strlen(INIT_VECTOR_STR)

void set_config(char* key_path, char* input_path, char* output_path, char* init_vector_path){
    FILE* fd = fopen("./src/Partie1/config.txt", "wb");

    fwrite(KEY_STR, sizeof(char), KEY_STR_LENGTH, fd);
    fwrite(key_path, sizeof(char), strlen(key_path), fd);
    fwrite("\n", sizeof(char), 1, fd);
    
    fwrite(INPUT_STR, sizeof(char), INPUT_STR_LENGTH, fd);
    fwrite(input_path, sizeof(char), strlen(input_path), fd);
    fwrite("\n", sizeof(char), 1, fd);

    fwrite(OUTPUT_STR, sizeof(char), OUTPUT_STR_LENGTH, fd);
    fwrite(output_path, sizeof(char), strlen(output_path), fd);
    fwrite("\n", sizeof(char), 1, fd);
    
    fwrite(INIT_VECTOR_STR, sizeof(char), INIT_VECTOR_STR_LENGTH, fd);
    if(init_vector_path!=NULL) fwrite(init_vector_path, sizeof(char), strlen(init_vector_path), fd);
    fwrite("\n", sizeof(char), 1, fd);
    
    fclose(fd);
}

void get_config(char* key_path, char* input_path, char* output_path, char* init_vector_path){
    FILE* fd = fopen("./src/Partie1/config.txt", "rb");

    
    fseek(fd, KEY_STR_LENGTH, SEEK_CUR);
    fread(key_path, sizeof(char), strlen(key_path), fd);
    fseek(fd, 1, SEEK_CUR); // \n
    
    fseek(fd, INPUT_STR_LENGTH, SEEK_CUR);
    fread(input_path, sizeof(char), strlen(input_path), fd);
    fseek(fd, 1, SEEK_CUR); // \n

    fseek(fd, OUTPUT_STR_LENGTH, SEEK_CUR);
    fread(output_path, sizeof(char), strlen(output_path), fd);
    fseek(fd, 1, SEEK_CUR); // \n
    
    fseek(fd, INIT_VECTOR_STR_LENGTH, SEEK_CUR);
    if(init_vector_path!=NULL) fread(init_vector_path, sizeof(char), strlen(init_vector_path), fd);
    fseek(fd, 1, SEEK_CUR); // \n

    fclose(fd);
}