#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>

#define TAILLE_CLEF 4 // longueur de la clé
#define MAX_CARACTERES 62 // nombre de caractères admissibles pour chaque valeur de la clé

char **clef; // tableau permettant de stocker les clés candidates
int sizes[TAILLE_CLEF]; 
char result[TAILLE_CLEF];                 

// fonction permettant de vérifier si un caractère est imprimable
int isAdmissibleChar(int c) {
  return isalnum(c) || isspace(c) || ispunct(c);
}

// fonction permettant de générer les caractères admissibles pour chaque indice de la clé
void clefAdmissible(char *chiffre, char **clef) {
    char clefCharacters[MAX_CARACTERES] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int tailleChiffre = strlen(chiffre);

    for (int i = 0; i < TAILLE_CLEF; i++) {
        int m = 0;
        for (int k = 0; k < MAX_CARACTERES; k++) {
            int estValide = 1; 
            for (int l = 0; estValide && (i + TAILLE_CLEF * l) < tailleChiffre; l++) {
                if (!isAdmissibleChar(clefCharacters[k] ^ chiffre[i + TAILLE_CLEF * l])) {
                    estValide = 0; // en cas de caractère invalide, on arrête la boucle et on sort
                }
            }
            if (estValide) {
                clef[i][m++] = clefCharacters[k]; // on ajoute le caractère valide à l'ensemble clef[i]
            }
        }
    }
}


// fonction pour générer toutes les combinaisons cartésiennes des ensembles admissibles
void cartesianProductRecursive(char **sets, int *sizes, char *result, int index, int n, FILE*file) {
    if (index == n) {
        fwrite(result,sizeof(char),TAILLE_CLEF,file);
        fputs("\n",file);
        return;
    }

    for (int i = 0; i < sizes[index]; i++) {
        result[index] = sets[index][i];
        cartesianProductRecursive(sets, sizes, result, index + 1, n,file);
    }
}

// fonction xor "ou exclusif"
void xor(const char *msg, unsigned char* key, char *crypted, int len_msg) {

    int len_key = strlen((char*)key);
    int index_key = 0;
    for (int i = 0; i < len_msg; i++){    
        crypted[i] = msg[i] ^ key[index_key];
        index_key = (index_key + 1) % len_key;
    }
}

// fonction pemettant de tester le xor
void test_xor(const char *msg) {
    int msg_length = strlen(msg);
    char crypte[msg_length + 1];
    char decrypte[msg_length + 1];
    int nbre=0;

    printf("Message d'origine : %s\n", msg);
    xor(msg, "cleC", crypte, msg_length);

    printf("Message crypte :");
    // on crypte le message et on l'affiche en héxadécimal
    for (size_t i = 0; i < strlen(msg); i++) {
        printf("%02x", (unsigned char)crypte[i]);
    }
    printf("\n");
    // on décrypte le message 

    xor(crypte, "cleC", decrypte, msg_length);
    printf("Message decrypte : %s\n", decrypte);

    clef = (char**)malloc(TAILLE_CLEF*(sizeof(char*)));
    for (int i=0; i<TAILLE_CLEF;i++){
      clef[i] = (char*) malloc(MAX_CARACTERES);
    }
    clefAdmissible(crypte,clef);
    for (int i = 0; i < TAILLE_CLEF ; i++){
      nbre=0;
      printf("clef[%d] : [", i);
      for (int k = 0; k < MAX_CARACTERES; k++){
        if (clef[i][k] != '\0'){
          printf("%c, ",clef[i][k]);
          nbre++;
        }
      }
      sizes[i] = nbre;
      printf("]\n");
    }
    FILE *file = fopen("clefs_candidates.txt", "w");
    cartesianProductRecursive(clef, sizes, result, 0, TAILLE_CLEF, file);
    fclose(file);
}


int main(int argc, char *argv[]) {
    if (argc != 2){
      fprintf(stderr, "Usage: %s <texte_non_crypte.txt>\n", argv[0]);
      exit(1);
    }

    FILE *fd;
    ssize_t bytesRead;
    char msg[1024];

    if ((fd=fopen(argv[1],"r")) == NULL){
      printf("Echec ouvert fichier\n");
      exit(EXIT_FAILURE);
    }

    if (fgets(msg, sizeof(msg), fd) == NULL) {
        printf("Echec de lecture du fichier\n");
        fclose(fd);
        exit(EXIT_FAILURE);
    }
    test_xor(msg);
    fclose(fd);
    return 0;
}