#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "dh_gen_group.h"
#include "dh_prime.h"

// Crée le dossier tmp/ s'il n'existe pas
void create_tmp_dir() {
        struct stat dir_stat;
        char *tmp_dir = "tmp/";

        if (stat(tmp_dir, &dir_stat) != 0)
                if (mkdir(tmp_dir, 0755)) { // 0755 : Owner -> Read & Write, Group -> Read only, Others -> Read only
                        perror("Erreur : mkdir\n");
                        exit(1);
                }
}

FILE *create_file(char *file_name) {
        FILE *file;
        char *file_dir = "tmp/";
        char file_path[strlen(file_dir) + strlen(file_name)];

        strcpy(file_path, "tmp/");
        strcat(file_path, file_name);

        create_tmp_dir(file_dir);

        if ((file = fopen(file_path, "w")) == NULL) {
                perror("Erreur : fopen\n");
                exit(2);
        }

        return file;
}

void dh_gen_group(char *file_name) {
        FILE *output_file = create_file(file_name);
        
        srand(time(NULL));
        generate_shared_key(MIN_PRIME, MAX_PRIME, output_file);

        if (fclose(output_file) != 0) {
                perror("Erreur : fclose\n");
                exit(3);
        }
}
