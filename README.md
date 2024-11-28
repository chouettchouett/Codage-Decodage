# Codage-Decodage
## Informations
Projet_avancé

L'ensemble du projet est à but de faire le sujet/travail demandé sur la matière Projet avancée.

Le projet importeras des fonctionnalités de codages-décodage, Cryptographie symétrique, test.

### Participants et occupations principales :
- Marcq Léo                => Test , Visibilité, cohérence
- CENDREZ Enzo             => Partie 1
- BARBERO Kylian           => Partie 2, Makefile, main
- Bertrand--Torres Victor  => Partie 3

## Utilisation
### [Makefile](Makefile)
- `make` - Pour compiler le programme complet
- `make partX` - Pour compiler seulement la partie X du programme
- `make clean` - Pour supprimer les fichiers objets, les fichiers temporaires et les exécutables
- `make clean_tmp` - Pour supprimer seulement les fichiers objets et les fichiers temporaires.

### [Partie 1](src/Partie1)
TODO

### [Partie 2](src/Partie2)
C :
- `./dh_gen_group -o [output]`
- `./dh_gen_group -h` - Pour afficher l'aide du programme C.
- `./dh_gen_group -t` - Pour lancer les tests du programme C.

Python :
- `python3 dh_genkey.py -i [input] -o [output]`
- `python3 dh_genkey.py -h` - Pour afficher l'aide du programme python.
- `python3 tests_dh_simulation.py` - Pour lancer les tests du programme python.

### [Partie 3](src/Partie3)
TODO
