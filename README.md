# Codage-Decodage
Projet_avancé
(Je finirais ça un peu plus tard (Léo) https://docs.github.com/fr/repositories/managing-your-repositorys-settings-and-features/customizing-your-repository/about-readmes T)

L'ensemble du projet est à but de faire le sujet/travail demandé sur la matière Projet avancée.

Le projet importeras des fonctionnalités de codages-décodage, Cryptographie symétrique..

prise en main:...

obtenir de l'aide :...

Participant du projet:        occupation principale:
Marcq Léo                => Visibilité, cohérence
Bertrand--Torres Victor  => Partie 3
BARBERO Kylian           => Partie 2 , makefile
CENDREZ Enzo             => Partie 1

## Makefile

   ### Utilisation
Commandes :
- `make` - Pour compiler le programme complet
- `make partX` - Pour compiler la partie X du programme et la tester
- `make clean` - Pour supprimer les .o et exécutables

### Organisation
Pour l'instant, chaque partie possède :
 - **main_partX.c** : qui servira lorsque l'on compile seulement cette partie pour la tester. Vous y mettez tout ce que vous voulez :D
 - **partX.c** + **partX.h** : qui contiendront les fonctions nécessaires au fonctionnement du programme principal. C'est ce .h qui sera inclus dans main.c (avec celui des autres parties).
