#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>

void help(FILE *log_file);

void list_keys(FILE *log_file);

void gen_key_main(FILE *log_file, bool dh, int n);

void del_key(FILE *log_file, int key_to_del);

void encrypt(FILE *log_file, char *input, char *output, int key_nb, char *method, char *vect);

void decrypt(FILE *log_file, char *input, char *output, int key_nb, char *method, char *vect);

#endif