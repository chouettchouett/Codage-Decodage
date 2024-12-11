#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "commands.h"
#include "utils/utils.h"
#include "utils/keys_utils.h"
#include "Partie1/sym_crypt_func.h"
//#include "Partie3/?.h"


void help(FILE *log_file) {
    // Affichage de l'aide
    print_and_log("Commandes :\n"
                  "\t- help : donne la liste des commandes\n"
                  "\t- gen-key <n> ou gen-key -dh : génère une clef de longueur n OU génère une clef en simulant un échange de Diffie-Hellman (-dh)\n"
                  "\t- gen-key <n> : génère une clef de longueur n\n"
                  "\t- del-key <n_key> : supprime la clef n° <n_key>\n"
                  "\t- encrypt <in> <out> <key> <method : xor OU mask OU cbc> [<vecteurd0 initialisation>]\n"
                  "\t- decrypt <in> <out> <key> <method : xor OU mask OU cbc> [<vecteurd0 initialisation>]\n"
                  "\t- crack <in> <out> <length> <dico>\n"
                  "\t- quit\n", false, true, log_file);
}

void list_keys(FILE *log_file) {
    FILE *key_list_file;
    char key[KEY_MAX_SIZE];
    int key_used = 0;
    int status;
    char msg[sizeof(int) + KEY_MAX_SIZE + 46];

    if ((key_list_file = open_file_read("keys_list", TMP)) == NULL) {
        print_and_log("Aucune clef n'a été générée pour le moment.\n", false, true, log_file);
        return;
    }

    print_and_log("Clefs générées :\n", false, true, log_file);

    // Lecture ligne par ligne des clefs
    status = fscanf(key_list_file, "%s %d", key, &key_used);
    for (int i = 1; status == 2; i++) {
        sprintf(msg, "\t- clef n°%d : %s, déjà utilisée : ", i, key);
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

void gen_key_main(bool dh, int n, FILE *log_file) {
    if (!dh && (n < 1 || n > KEY_MAX_SIZE)) {
        print_and_log("Erreur : longueur de la clef incorrecte (minimum : 0, maximum : 100)\n", false, true, log_file);
        return;
    }

    char key[KEY_MAX_SIZE + 1];

    if (!dh) // Génération de la clef avec la partie 1
        strcpy(key, gen_key(n));
    else // Génération de la clef avec la partie 2
        gen_key_dh(key, log_file);

    // Ajout de la clef dans la liste des clefs
    FILE *key_list_file = create_file("keys_list", false, TMP);
    fprintf(key_list_file, "%s %d\n", key, 0);

    if (fclose(key_list_file) != 0) {
        print_and_log("Erreur : gen_key_main -> fclose()\n", true, true, log_file);
        exit(2);   
    }

    // Affichage de la nouvelle clef
    char msg[KEY_MAX_SIZE + 45];
    strcpy(msg, "Nouvelle clef (= ");
    strcat(msg, key);
    strcat(msg, ") générée avec succès.\n");

    print_and_log(msg, false, true, log_file);  
}

void del_key(int key_to_del, FILE *log_file) {
    if (key_to_del < 1 || key_to_del > get_nb_keys(log_file)) {
        print_and_log("Numéro de clef incorrect, faire list-keys pour voir les clefs disponibles.\n", false, true, log_file);
        return;
    }

    FILE *key_list_file = open_file_read("keys_list", TMP);
    FILE *temp_file = create_file("keys_list_temp", true, TMP);
    char line[KEY_MAX_SIZE + 3];
    int i = 1;

    // Ecrit dans un fichier temporaire les autres clefs
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
    remove("tmp/keys_list");
    rename("tmp/keys_list_temp", "tmp/keys_list");

    print_and_log("Clef supprimée avec succès.\n", false, true, log_file);
}

int get_crypt_method(char *method) {
    int crypt_method = -1;

    if (strcmp(method, "xor") == 0)
        crypt_method = 0;
    else if (strcmp(method, "mask") == 0)
        crypt_method = 1;
    else if (strcmp(method, "cbc") == 0)
        crypt_method = 2;

    return crypt_method;
}

void encrypt(char *input, char *output, int key_nb, char *method, char *vect, FILE *log_file) {    
    char *key;
    
    // Récupère la clef choisie
    if ((key = get_key(key_nb, log_file)) == NULL)
        return;

    switch (get_crypt_method(method)) {
        case 0: // Appel encrypt XOR
            printf("Appel XOR\n");
            break;
        case 1: // Appel encrypt MASK
            printf("Appel MASK\n");
            break;
        case 2: // Appel encrypt CBC
            printf("Appel CBC\n");
            break;   
        default:
            print_and_log("Erreur : Méthode entrée incorrecte. Entrer \"help\" pour plus d'information .\n", true, true, log_file);
            return;
    }
    
    // Enregistrer dans la liste que la clef à été utilisée
    set_key_used(key_nb, key, log_file);

    free(key);

    print_and_log("Cryptage effectué avec succès.\n", false, true, log_file);
}

void decrypt(char *input, char *output, int key_nb, char *method, char *vect, FILE *log_file) {
    char *key;
    
    // Récupère la clef choisie
    if ((key = get_key(key_nb, log_file)) == NULL)
        return;

    switch (get_crypt_method(method)) {
        case 0: // Appel decrypt XOR
            printf("Appel XOR\n");
            break;
        case 1: // Appel decrypt MASK
            printf("Appel MASK\n");
            break;
        case 2: // Appel decrypt CBC
            printf("Appel CBC\n");
            break;   
        default:
            print_and_log("Erreur : Méthode entrée incorrecte. Entrer \"help\" pour plus d'information .\n", true, true, log_file);
            return;
    }

    free(key);

    print_and_log("Décryptage effectué avec succès.\n", false, true, log_file);
}

void crack(char *input, char *output, int length, char *dico, FILE *log_file) {
    printf("Fonction crack - input : %s, output : %s, length : %d, dico : %s\n", input, output, length, dico); // Temp
}