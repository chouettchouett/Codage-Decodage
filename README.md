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

### [Programme principal](src/main.c)
`./projet_avance_automne_2024`

#### Commandes :
- `help` : donne la liste des commandes
- `list-keys` : donne la liste des clefs générées et disponibles et indique celle qui ont déjà été utilisée
- (TODO) `gen-key <n>` : génère une clef de longueur n
- `del-key <n_key>` : supprime la clef n° <n_key>
- (TODO) `encrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]`
- (TODO) `decrypt <in> <out> <key> <method> [<vecteurd0 initialisation>]`
- (TODO) `crack <in> <out> <length> <dico>`
- `quit`


### [Partie 1](src/Partie1)
TODO

### [Partie 2](src/Partie2)
#### C :
- `./dh_gen_group -o <output>`
- `./dh_gen_group -h` - Pour afficher l'aide.
- `./dh_gen_group -t` - Pour lancer les tests.

#### Python :
- `python3 dh_genkey.py -i <input> -o <output>`
- `python3 dh_genkey.py -h` - Pour afficher l'aide.
- `python3 tests_dh_simulation.py` - Pour lancer les tests.

### [Partie 3](src/Partie3)
TODO
