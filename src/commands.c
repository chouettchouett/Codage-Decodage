#include <stdio.h>
#include <stdlib.h>

#include "commands.h"
#include "utils.h"

#define KEYS_LIST_FILE_NAME "keys-list"


void help(FILE *log_file) {
    print_and_log("Commandes :\n"
                  "\t- help : donne la liste des commandes\n"
                  "\t- list-keys : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée\n"
                  "\t- gen-key <n> : génère une clef de longueur n\n"
                  "\t- del-key <key> : supprime la clef < key >\n"
                  "\t- encrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]\n"
                  "\t- decrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]\n"
                  "\t- crack <in> <out> <length> <dico>\n"
                  "\t- quit\n", false, log_file);
}

void list_keys(FILE *log_file) {
    FILE *key_list_file;

    if ((key_list_file = fopen(KEYS_LIST_FILE_NAME, "r")) == NULL) {
        print_and_log("Aucune clé n'a été générée pour le moment.\n", false, log_file);
        return;
    }

    // TODO Afficher les clés

    if (fclose(key_list_file) != 0) {
            print_and_log("Erreur : list_keys -> fclose()\n", true, log_file);
            exit(1);
    }
}