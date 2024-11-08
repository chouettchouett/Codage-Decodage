#ifndef DH_GEN_GROUP_TEST_H
#define DH_GEN_GROUP_TEST_H

// Fonction pour tester l'option -o avec un nom de fichier valide
void test_option_o();

// Fonction pour tester l'option -o sans nom de fichier
void test_option_o_no_filename();

// Fonction pour tester l'option -h
void test_option_h();

// Fonction pour vérifier que p est un nombre premier de Sophie Germain
int is_prime(int n);
int is_sophie_germain_prime(int p);
void test_prime_sophie_germain();

// Fonction pour tester que g est un générateur de Z/pZ
int is_generator(int g, int p);
void test_generator();

#endif // DH_GEN_GROUP_TEST_H
