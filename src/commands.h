#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include <stdbool.h>

void help(FILE *log_file);

void list_keys(FILE *log_file);

void gen_key_main(bool dh, int n, FILE *log_file);

void del_key(int key_to_del, FILE *log_file);

void encrypt(char *input, char *output, int key_nb, char *method, char *vect, FILE *log_file);

void decrypt(char *input, char *output, int key_nb, char *method, char *vect, FILE *log_file);

void crack(char *input, char *output, int key_length, char *dico, FILE *log_file);

#endif