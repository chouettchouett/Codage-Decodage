#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdbool.h>

enum File_Type {
    TMP,
    LOG,
    OUTPUT
};

// Ouvre un fichier dans le dossier TMP ou LOG ou OUTPUT, et le crée s'il n'existe pas
FILE *create_file(char *file_name, bool overwrite_file, enum File_Type file_type);

// Ouvre un fichier en lecture dans le dossier TPM ou LOG ou OUTPUT
FILE *open_file_read(char * file_name, enum File_Type file_type);

// Ecrit le message au format "[%d/%m/%Y-%H:%M:%S] - <msg>" dans le fichier log
void log_msg(char *msg, bool input, bool log_time, FILE *log_file);

// Affiche et ecrit le message au format "[%d/%m/%Y-%H:%M:%S] - <msg>" dans le fichier log
void print_and_log(char *msg, bool error, bool log_time, FILE *log_file);

#endif
