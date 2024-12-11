#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "keys_utils.h"
#include "utils.h"
#include "../Partie2/dh_gen_group.h"


void gen_key_dh(char *key, FILE *log_file) {
        // Appel des fonctions et du script python de la partie 2
        dh_gen_group("dh_group_tmp");

        if (system("python3 dh_genkey.py -i dh_group_tmp -o dh_key_tmp > /dev/null") != 0) {
            print_and_log("Erreur : gen_key_main -> dh_genkey.py", true, true, log_file);
            exit(1);
        }

        // Récupération de la clef générée
        FILE * dh_key_file = open_file_read("dh_key_tmp", TMP);
        
        if (fscanf(dh_key_file, "%s", key) != 1) {
            print_and_log("Erreur : gen_key_main -> fscanf()\n", true, true, log_file);
            exit(2);   
        }   
        
        if (fclose(dh_key_file) != 0) {
            print_and_log("Erreur : gen_key_main -> fclose()\n", true, true, log_file);
            exit(3);   
        }
}

int get_nb_keys(FILE *log_file) {
    FILE *key_list_file;
    char key[KEY_MAX_SIZE];
    int key_used;
    int nb_key = 0;

    if ((key_list_file = open_file_read("keys_list", TMP)) == NULL) {
        return -1;
    }

    while (fscanf(key_list_file, "%s %d", key, &key_used) == 2)
        nb_key++;

    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : get_nb_key -> fclose()\n", true, true, log_file);
        exit(4);
    }   

    return nb_key;
}

// Renvoie la n ième clef de la liste des clefs
char *get_key(int key_nb, FILE *log_file) {
    if (key_nb < 1 || key_nb > get_nb_keys(log_file)) {
        print_and_log("Numéro de clef incorrect, faire list-keys pour voir les clefs disponibles.\n", false, true, log_file);
        return NULL;
    }    

    FILE *key_list_file;
    char key_temp[KEY_MAX_SIZE];
    int used_temp;

    if ((key_list_file = open_file_read("keys_list", TMP)) == NULL) {
        print_and_log("La clef n'existe pas car aucune clef n'a été générée pour le moment.\n", true, true, log_file);
        return NULL;
    }

    for (int i = 0; i != key_nb; i++) {
       if (fscanf(key_list_file, "%s %d", key_temp, &used_temp) != 2) {
            print_and_log("Erreur : clef introuvable dans le fichier.\n", true, true, log_file);
            fclose(key_list_file);
            exit(5);
        }
    }

    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : get_nb_key -> fclose()\n", true, true, log_file);
        exit(6);
    }

    char *key = malloc(strlen(key_temp) + 1);
    strcpy(key, key_temp);

    return key;
}

void set_key_used(int key_nb, char *key, FILE *log_file) {
    if (key_nb < 1 || key_nb > get_nb_keys(log_file)) {
        print_and_log("Numéro de clef incorrect, faire list-keys pour voir les clefs disponibles.\n", false, true, log_file);
        return;
    }

    FILE *key_list_file = open_file_read("keys_list", TMP);
    FILE *temp_file = create_file("keys_list_temp", true, TMP);
    char line[KEY_MAX_SIZE + 3];
    int i = 1;

    // Ecrit dans un fichier temporaire la liste mise à jour
    while (fgets(line, sizeof(line), key_list_file) != NULL) {
        if (i != key_nb)
            fputs(line, temp_file);
        else {
            strcpy(line, key);
            strcat(line, " 1\n");
            fputs(line, temp_file);
        }
        i++;
    }

    // Supprime l'ancienne liste et renomme la nouvelle
    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : del_key -> fclose(key_list_file)\n", true, true, log_file);
        exit(7);
    }
    if (fclose(temp_file) != 0) {
        print_and_log("Erreur : del_key -> fclose(temp_file)\n", true, true, log_file);
        exit(8);
    }
    remove("tmp/keys_list");
    rename("tmp/keys_list_temp", "tmp/keys_list");
}