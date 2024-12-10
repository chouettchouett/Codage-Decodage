#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>

void help(FILE *log_file);

void list_keys(FILE *log_file);

// gen_key

void del_key(FILE *log_file, char **args);

#endif