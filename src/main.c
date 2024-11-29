#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#include "Partie1/sym_crypt_func.h"
#include "Partie2/dh_gen_group.h"
//#include "Partie3/?.h"


int command_prompt(/* Ajouter pointeur d'arguments, */ FILE *log_file) {
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

int menu(FILE *log_file) {
    bool quit = false;

    printf("Projet avancé Automne 2024 - Equipe 13\n");

    while(!quit) {
        switch (command_prompt(/* char **args ?,*/ log_file)) {
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
                print_and_log("Erreur : Mauvaise entrée. Entrer \"help\" pour plus d'information.\n", true, log_file);
                exit(0); // Temporaire, pour éviter la boucle infinie :)
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
