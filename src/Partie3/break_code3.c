#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fonctions_annexes.h"  // Déclaration des fonctions annexes utilisées pour le déchiffrement et la gestion des fichiers
#include "tad_liste.h"  // Déclaration des fonctions pour la gestion des listes chaînées

// Fonction principale pour le déchiffrement avec les clés XOR
int break_code_c3(const char **keys_tab, int keys_count, const char *dictionary, const char *text) {
    int word_count = 0;  // Variable pour compter le nombre de mots dans le texte déchiffré
    FILE *decrypt_text;  // Fichier pour stocker le texte déchiffré temporairement
    int score;  // Score qui évalue la qualité de chaque déchiffrement (basé sur la correspondance avec le dictionnaire)
    Node* head = NULL;  // Liste chaînée pour stocker les résultats de chaque déchiffrement (clé et score)

    // Boucle sur chaque clé du tableau pour essayer chaque clé de déchiffrement
    for (int i = 0; i < keys_count; i++) {
        score = 0;  // Initialisation du score pour cette clé

        // Appel de la fonction `xor_crypt_decrypt` pour déchiffrer le texte en utilisant la clé actuelle
        xor_crypt_decrypt(text, "message.txt", keys_tab[i]);

        // Ouverture du fichier contenant le texte déchiffré
        decrypt_text = fopen("message.txt", "r");
        if (decrypt_text == NULL) {
            // Si l'ouverture du fichier échoue, afficher un message d'erreur et retourner
            printf("Error: Can't open file 'decrypt_text.txt'.\n");
            return 1;
        }

        // Convertir le texte déchiffré en un tableau de mots (word_tab) et compter le nombre de mots
        char **word_tab = text_to_tab("message.txt", &word_count);
        fclose(decrypt_text);  // Fermer le fichier après avoir lu son contenu

        // Pour chaque mot dans le tableau, augmenter le score si le mot est trouvé dans le dictionnaire
        for (int j = 0; j < word_count; j++) {
            score += search_in_dictionary(word_tab[j], dictionary);  // Recherche du mot dans le dictionnaire
        }

        // Ajouter l'entrée de la clé et son score dans la liste chaînée
        head = add_node(head, keys_tab[i], score);

        // Libérer la mémoire allouée pour le tableau de mots
        free_word_tab(word_tab, word_count);
    }

    // Afficher la liste des résultats (clé, score) triée
    print_list(head);

    // Libérer la mémoire utilisée pour la liste chaînée
    free_list(head);

    return 0;  // Indiquer que l'exécution s'est bien passée
}

int main(int argc, const char *argv[]) {
    // Vérification que le nombre d'arguments passés est suffisant
    if (argc < 4) {
        // Si le nombre d'arguments est insuffisant, afficher un message d'utilisation et retourner une erreur
        printf("Usage: %s <dictionnaire.txt> <message_crypte.txt> <clefs_candidates_c2.txt>\n", argv[0]);
        return 1;
    }

    // Récupération des chemins des fichiers à partir des arguments de la ligne de commande
    const char *dictionary = argv[1];  // Chemin du fichier du dictionnaire
    const char *text = argv[2];  // Chemin du fichier du texte à déchiffrer
    const char *keys_file = argv[3];  // Chemin du fichier contenant les clés

    // Lire les clés depuis le fichier spécifié
    int keys_count = 0;  // Compteur de clés lues
    char **keys_tab = read_keys(keys_file, &keys_count);  // Lecture des clés dans le fichier
    if (keys_tab == NULL) {
        // Si l'ouverture ou la lecture des clés échoue, retourner une erreur
        return 1;
    }

    // Appeler la fonction de déchiffrement avec les clés, le dictionnaire et le texte
    break_code_c3((const char **)keys_tab, keys_count, dictionary, text);

    // Libérer la mémoire allouée pour les clés
    for (int i = 0; i < keys_count; i++) {
        free(keys_tab[i]);  // Libérer chaque clé
    }
    free(keys_tab);  // Libérer le tableau de clés

    return 0;  // Retourner 0 pour indiquer que tout s'est bien passé
}
