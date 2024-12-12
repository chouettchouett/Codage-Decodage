# Cryptographie symétrique
## Informations générales
Dans le cadre de l'UE Projet avancé (automne 2024), nous devons produire un programme proposant :
- Du chiffrage/déchiffrage avec une clef symétrique (partie 1).
- Une simulation d'un échange de Diffie-Hellman (partie 2).
- Des attaques sur un message chiffré (partie 3).
- Des jeux de tests pour chaque parties.

## Participants
- [Marcq Léo](https://github.com/chouettchouett) : tests, visibilité et cohérence.
- [BARBERO Kylian](https://github.com/kylian-bbo) : programme principal, partie 2, Makefile, README et organisation du dépôt.
- [CENDREZ Enzo](https://github.com/MKZenit) : partie 1.
- [Bertrand--Torres Victor](https://github.com/victorgh31) : partie 3.

## Utilisation
### [Makefile](Makefile)
- `make` - Pour compiler le programme complet.
- `make partX` - Pour compiler seulement la partie X du programme.
- `make clean` - Pour supprimer les fichiers objets, les fichiers temporaires et les exécutables.
- `make clean_tmp` - Pour supprimer seulement les fichiers objets et les fichiers temporaires.

### [Programme principal](src/main.c)
`./projet_avance_automne_2024`

#### Commandes :
- `help` : donne la liste des commandes.
- `list-keys` : donne la liste des clefs générées et disponibles et indique celles qui ont déjà été utilisée.
- `gen-key <n>` OU `gen-key -dh` : génère une clef de longueur n OU génère une clef en simulant un échange de Diffie-Hellman (-dh).
- `del-key <n_key>` : supprime la clef n° <n_key>.
- `encrypt <in> <out> <key> <method : xor OU mask OU cbc> [<vecteurd0 initialisation>]`
- `decrypt <in> <out> <key> <method : xor OU mask OU cbc> [<vecteurd0 initialisation>]`
- `crack <in> <out> <length> <dico>`
- `quit`

### [Partie 1](src/Partie1)
`./crypto_sym -i <input> -o <output> -k <key> -m <method : xor OU mask OU cbc> [-v <vect>] [-l <log>] [-h]`

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
`./break_code -i <input> -m <method : c1 OU all> -k <key length> [-d <dict>] [-h] [-l <log>]` 