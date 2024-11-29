#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"

#define TMP_DIR "tmp/"
#define LOG_DIR "logs/"
#define OUTPUT_DIR TMP_DIR


// Renvoie le dossier dans lequel sera créé le fichier en fonction de son type
char *get_file_dir(enum File_Type file_type) {
    char *dir;

    switch (file_type) {
            case TMP:
                dir = TMP_DIR;
                break;
            case LOG:
                dir = LOG_DIR;
                break;
            case OUTPUT:
                dir = OUTPUT_DIR;
                break;
            default:
                fprintf(stderr, "Erreur : switch create_dir\n");
                exit(1);
                break;
    }
    return dir;
}

// Crée le dossier s'il n'existe pas
void create_dir(enum File_Type file_type) {
        struct stat dir_stat;
        char *dir = get_file_dir(file_type);

        if (stat(dir, &dir_stat) != 0)
                if (mkdir(dir, 0755)) { // 0755 : Owner -> Read & Write, Group -> Read only, Others -> Read only
                        fprintf(stderr, "Erreur : mkdir\n");
                        exit(2);
                }
}

FILE *create_file(char *file_name, bool overwrite_file, enum File_Type file_type) {
        FILE *file;
        char *file_dir = get_file_dir(file_type);
        char file_path[strlen(file_dir) + strlen(file_name)];

        strcpy(file_path, file_dir);
        strcat(file_path, file_name);

        create_dir(file_type);

        if (overwrite_file) {
            if ((file = fopen(file_path, "w")) == NULL) {
                    fprintf(stderr, "Erreur : fopen\n");
                    exit(3);
            }
        }
        else {
            if ((file = fopen(file_path, "a")) == NULL) {
                fprintf(stderr, "Erreur : fopen\n");
                exit(4);
            }
        }

        return file;
}

// Ecrit le message au format "[%d/%m/%Y-%H:%M:%S] - <msg>" dans le fichier log
void log_msg(char *msg, FILE *log_file) {
    // Génération de l'heure
    char str_time[23];
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(str_time, sizeof(str_time), "[%d/%m/%Y-%H:%M:%S] ", tm);

    // Création du message de log
    char log_msg[strlen(str_time) + strlen(msg)];    
    strcpy(log_msg, str_time);
    strcat(log_msg, msg);

    fprintf(log_file, log_msg);
}

void print_and_log(char *msg, bool error, FILE *log_file) {
    if (error)
        fprintf(stderr, msg);
    else
        printf("%s", msg);
    
    log_msg(msg, log_file);
}
