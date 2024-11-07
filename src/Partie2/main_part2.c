#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dh_gen_group.h"


void option_error() {
        printf("Erreur : Mauvais arguments. Faire -h pour plus d'aide.\n");
}

void option_help() {
    printf("Usage :\n\
            dh_gen_group [option]\n");
    printf("Options :\n\
            \t-o <nom_de_fichier> : Nom du fichier où les résultats p et g seront écrits.\n\
            \t-h : Affiche cette aide.\n");
}

int main(int argc, char **argv) {

    if (argc == 2 && strcmp(argv[1], "-h") == 0)
        option_help();
    
    else if (argc == 3 && strcmp(argv[1], "-o") == 0)
        dh_gen_group(argv[2]);

    else {
        option_error();
        return -1;
    }

    return 0;
}