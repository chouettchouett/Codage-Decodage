#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "commands.h"
#include "Partie1/sym_crypt_func.h"
#include "Partie2/dh_gen_group.h"
//#include "Partie3/?.h"

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


int command_prompt(char **args, int *args_nb, FILE *log_file) {
    char input[MAX_INPUT_LENGTH];
    char *command;
    char *arg;
    int choice = -1;
    int i = 0;

    // Lire l'entrée de l'utilisateur
    printf("> ");
    fgets(input, MAX_INPUT_LENGTH, stdin);
    log_msg(input, true, true, log_file);

    // Récupérer la commande
    input[strcspn(input, "\n")] = '\0';
    command = strtok(input, " ");

    for (int j = 0; j < NB_COMMANDS && choice == -1; j++)
        if (strcmp(command, commands[j]) == 0)
            choice = j;
    
    // Récupérer les arguments si la commande est correcte
    if (choice != -1) {
        while (i < MAX_ARGS && (arg = strtok(NULL, " ")) != NULL) {
            args[i] = arg;
            i++;
        }

        *args_nb = i;

        // Effacer les derniers arguments
        while (i < MAX_ARGS) {
            args[i] = NULL;
            i++;
        }
    }
    else
        *args_nb = -1;

    return choice;
}

void error_wrong_command(FILE *log_file) {
    print_and_log("Erreur : Mauvaise entrée. Entrer \"help\" pour plus d'information.\n", true, true, log_file);
}

int menu(FILE *log_file) {
    bool quit = false;
    char *args[MAX_ARGS];
    int args_nb = -1;
    
    print_and_log("Projet avancé Automne 2024 - Equipe 13\n", false, true, log_file);

    while(!quit) {
        switch (command_prompt(args, &args_nb, log_file)) {
            case 0: //quit
                quit = true;
                break;
            case 1: //list-keys
                list_keys(log_file);
                break;
            case 2: //gen-key
                if (args_nb == 1) {
                    int n;
                    bool dh = false;

                    if (strcmp(args[0], "-dh") == 0) {
                        n = 0;
                        dh = true;
                    }
                    else
                        n = strtol(args[0], NULL, 10);
                    
                    gen_key_main(log_file, dh, n);
                }
                else
                    error_wrong_command(log_file);
                break;
            case 3: //del-key
                if (args_nb == 1) {
                    int key_to_del = strtol(args[0], NULL, 10);
                    del_key(log_file, key_to_del);
                }
                else
                    error_wrong_command(log_file);
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
                error_wrong_command(log_file);
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
