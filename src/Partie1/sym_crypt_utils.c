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

void GetConfigLine(char** path, FILE* fd){
    const int maxLineLength = 199;
    int curIndex = 0;
    char curChar;
    char* p = malloc(sizeof(char)*(maxLineLength+1));

    fread(&curChar, sizeof(char), 1, fd);
    p[curIndex] = curChar;
    while((curIndex<maxLineLength)
        && ((curChar!='\n')&&(curChar!='\0')) ){
        curIndex++;
        fread(&curChar, sizeof(char), 1, fd);
        p[curIndex] = curChar;
    }
    p[curIndex] = '\0';
    *path = p;
}


void get_config(char* key_path, char* input_path, char* output_path, char* init_vector_path){
    FILE* fd = fopen("./src/Partie1/config.txt", "rb");

    char* line;
    int len;

    GetConfigLine(&line, fd);
    strcpy(key_path, line+KEY_STR_LENGTH);
    
    GetConfigLine(&line, fd);
    strcpy(input_path, line+INPUT_STR_LENGTH);
    
    GetConfigLine(&line, fd);
    strcpy(output_path, line+OUTPUT_STR_LENGTH);

    if(init_vector_path!=NULL){
        GetConfigLine(&line, fd);
        len = strlen(line);
        *init_vector_path = malloc(sizeof(char)*(len-INIT_VECTOR_STR_LENGTH));
        strncpy(*init_vector_path, line+INIT_VECTOR_STR_LENGTH, len-INIT_VECTOR_STR_LENGTH-1);
        init_vector_path[len-INIT_VECTOR_STR_LENGTH]='\0';
    }

    fclose(fd);
}