#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "Partie1/sym_crypt_func.h"
#include "Partie2/dh_gen_group.h"
//#include "Partie3/?.h"

#define KEYS_LIST_FILE_NAME "keys-list"
#define MAX_INPUT_LENGTH 200 // ?
#define MAX_ARGS 5
#define NB_COMMANDS 8 // Toutes les commandes possibles :

const char *commands[] = {
    "quit",
    "list-keys",
    "gen-key",
    "del-key",
    "encrypt",
    "decrypt",
    "crack",
    "help"
};


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

int command_prompt(char **args, FILE *log_file) {
    char input[MAX_INPUT_LENGTH];
    char *command;
    char *arg;
    int choice = -1;
    int i = 0;

    // Lire l'entrée de l'utilisateur
    printf("> ");
    fgets(input, MAX_INPUT_LENGTH, stdin);
    log_msg(input, true, log_file);

    // Récupérer la commande
    input[strcspn(input, "\n")] = '\0';
    command = strtok(input, " ");

    for (int i = 0; i < NB_COMMANDS && choice == -1; i++)
        if (strcmp(command, commands[i]) == 0)
            choice = i;
    
    // Récupérer les arguments si la commande est correcte
    if (choice != -1) {
        while (i < MAX_ARGS && (arg = strtok(NULL, " ")) != NULL) {
            args[i] = arg;
            i++;
        }
        
        // Effacer les derniers arguments
        while (i < MAX_ARGS) {
            args[i] = NULL;
            i++;
        }
    }

    return choice;
}

int menu(FILE *log_file) {
    bool quit = false;
    char *args[MAX_ARGS];

    printf("Projet avancé Automne 2024 - Equipe 13\n");

    while(!quit) {
        switch (command_prompt(args, log_file)) {
            case 0: //quit
                quit = true;
                break;
            case 1: //list-keys
                list_keys(log_file);
                break;
            case 2: //gen-key
                printf("Choix 2\n"); // TEMPORAIRE
                break;
            case 3: //del-key
                printf("Choix 3\n"); // TEMPORAIRE
                break;
            case 4: //encrypt
                printf("Choix 4\n"); // TEMPORAIRE
                break;
            case 5: //decrypt
                printf("Choix 5\n"); // TEMPORAIRE
                break;
            case 6: //crack
                printf("Choix 6\n"); // TEMPORAIRE          
                break;
            case 7: //help
                help(log_file);
                break;
            default:
                print_and_log("Erreur : Mauvaise entrée. Entrer \"help\" pour plus d'information.\n", true, log_file);
        }
    }

    return 0;
}

int main () {
    FILE *log_file = create_file("log", false, LOG);
    
    menu(log_file);

    if (fclose(log_file) != 0) {
            fprintf(stderr, "Erreur : fclose(log_file)\n");
            exit(1);
    }

    return 0;
}
