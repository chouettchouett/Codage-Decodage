#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dh_gen_group.h"
#include "tests_dh_gen.h"

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

void tests() {
    test_option_o();
    test_option_o_no_filename();
    test_option_h();

    printf("Tous les tests ont été exécutés avec succès.\n");
}

int main(int argc, char **argv) {
    
    if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0)
            option_help();
        else if (strcmp(argv[1], "-t") == 0)
            tests();
        else {
            option_error();
            return -1; 
        }
    }

    else if (argc == 3 && strcmp(argv[1], "-o") == 0)
        dh_gen_group(argv[2]);

    else {
        option_error();
        return -1;
    }

    return 0;
}
