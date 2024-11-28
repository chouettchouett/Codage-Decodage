#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "Partie1/sym_crypt_func.h"
#include "Partie2/dh_gen_group.h"
//#include "Partie3/?.h"


void log_message(char *msg, FILE *log_file) {
    // TODO
}

void print_and_log_message(char *msg, FILE *log_file) {
    printf("%s", msg);
    log_message(msg, log_file);
}

int command_prompt(/* Ajouter pointeur d'arguments */) {
    int choice = -1;

    printf("> ");
    // TODO :
    // Lire la ligne
    // Récupérer la commande
    // Vérifier si la commande est correcte
    // Si oui, récupérer les arguments de la commande
    // Si les arguments sont corrects, on renvoie le numéro de commande et les arguments
    // Sinon on renvoie -1
    
    return choice;
}

int menu() {
    bool quit = false;

    printf("Projet avancé Automne 2024 - Equipe 13\n");

    while(!quit) {
        switch (command_prompt(/* char **args ?*/)) {
            case 0: //quit
                quit = true;
                break;
            case 1: //list-keys

                break;
            case 2: //gen-key

                break;
            case 3: //del-key
                
                break;
            case 4: //encrypt

                break;
            case 5: //decrypt

                break;
            case 6: //crack
            
                break;
            case 7: //help

                break;
            default:
                fprintf(stderr, "Erreur : Mauvaise entrée. Entrer \"help\" pour plus d'information.");
                exit(0); // Temporaire, pour éviter la boucle infinie :)
        }
    }

    return 0;
}

int main () {
    menu();
}