#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"
#include "utils.h"

#define KEY_MAX_SIZE 100


void help(FILE *log_file) {
    // Affichage de l'aide
    print_and_log("Commandes :\n"
                  "\t- help : donne la liste des commandes\n"
                  "\t- list-keys : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée\n"
                  "\t- gen-key <n> : génère une clef de longueur n\n"
                  "\t- del-key <key> : supprime la clef < key >\n"
                  "\t- encrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]\n"
                  "\t- decrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]\n"
                  "\t- crack <in> <out> <length> <dico>\n"
                  "\t- quit\n", false, true, log_file);
}

void list_keys(FILE *log_file) {
    FILE *key_list_file;
    char key[KEY_MAX_SIZE];
    int key_used = 0;
    int status;
    char msg[sizeof(int) + KEY_MAX_SIZE + 46];

    if ((key_list_file = open_file_read("keys-list", false, TMP)) == NULL) {
        print_and_log("Aucune clé n'a été générée pour le moment.\n", false, true, log_file);
        return;
    }

    print_and_log("Clés générées :\n", false, true, log_file);

    // Lecture ligne par ligne des clefs
    status = fscanf(key_list_file, "%s %d", key, &key_used);
    for (int i = 1; status == 2; i++) {
        sprintf(msg, "\t- Clé n°%d : %s, déjà utilisée : ", i, key);
        if (key_used)
            strcat(msg, "oui\n");
        else
            strcat(msg, "non\n");
        // Affichage de la clef  
        print_and_log(msg, false, false, log_file);

        status = fscanf(key_list_file, "%s %d", key, &key_used);
    }

    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : list_keys -> fclose()\n", true, true, log_file);
        exit(1);
    }
}

// Renvoie le nombre de clefs disponibles dans le fichier
int get_nb_keys(FILE *log_file) {
    FILE *key_list_file;
    char key[KEY_MAX_SIZE];
    int key_used;
    int nb_key = 0;

    if ((key_list_file = open_file_read("keys-list", false, TMP)) == NULL) {
        return -1;
    }

    while (fscanf(key_list_file, "%s %d", key, &key_used) == 2)
        nb_key++;

    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : get_nb_key -> fclose()\n", true, true, log_file);
        exit(2);
    }   

    return nb_key;
}

// TODO : gen-key

void del_key(FILE *log_file, int key_to_del) {
    if (key_to_del < 1 || key_to_del > get_nb_keys(log_file)) {
        print_and_log("Erreur : Numéro de clé incorrect, faire list-keys pour voir les clés disponibles.\n", true, true, log_file);
        return;
    }

    FILE *key_list_file = open_file_read("keys-list", false, TMP);
    FILE *temp_file = create_file("keys-list-temp", true, TMP);
    char line[KEY_MAX_SIZE + 3];
    int i = 1;

    // Ecrit dans un fichier temporaire les autres clés
    while (fgets(line, sizeof(line), key_list_file) != NULL) {
        if (i != key_to_del)
            fputs(line, temp_file);
        i++;
    }

    // Supprime l'ancienne liste et renomme la nouvelle
    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : del_key -> fclose(key_list_file)\n", true, true, log_file);
        exit(3);
    }
    if (fclose(temp_file) != 0) {
        print_and_log("Erreur : del_key -> fclose(temp_file)\n", true, true, log_file);
        exit(4);
    }
    remove("tmp/keys-list");
    rename("tmp/keys-list-temp", "tmp/keys-list");

    print_and_log("Clé supprimée avec succès.\n", false, true, log_file);
}