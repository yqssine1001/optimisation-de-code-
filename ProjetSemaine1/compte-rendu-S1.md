# Compte Rendu Semaine 1

## Correction Warning Compilation

```
>>> gcc -o dico dico.c -lm -Wall
dico.c: In function ‘insertDico’:
dico.c:200:9: warning: variable ‘newDictionaryPrevious’ set but not used [-Wunused-but-set-variable]
  200 |   dico* newDictionaryPrevious = (dico*) malloc(sizeof(dico));
      |         ^~~~~~~~~~~~~~~~~~~~~
```

L'erreur provient de la fonction `insertDico(dico** dictionary, mot_t* linkWord) ` qui pour spécification d'inserer un mot `linkWord` dans le dictionnaire `dictionary`.

Dans le code, on remarque que la variable `newDictionaryPrevious` est déclarée et initialisée mais jamais utilisée par la suite.

Après analyse, il semble que cette variable n'a pas été oubliée d'être utilisée, mais plutôt qu'elle ne sert à rien dans l'algorithme d'insertion et peut être supprimée.

ainsi la commande `gcc -o dico dico.c -lm -Wall` ne génère plus de warning.

> il y a aussi d'autres erreurs dans cette fonction
> - le fait que `newDictionary` est initialisé deux fois dans le cas, où le dictionnaire `dico` est NULL.
> - Aucune libération de mémoire n'est faite pour `newDictionary` (comme dans tous le code d'ailleurs).
> - l'allocation dynamique de `newDictionary` n'est pas nécessaire dans l'algorithme d'insertion, on pourrait directement utiliser un pointeur vers `dico`.
> Des structure de controle while sont utilisées alors que des structures if seraient plus appropriées.
> Ces erreurs n'ont pas été corrigées.
## Correction du segmantation fault

le programme principal `dico` génère une erreur de segmentation fault.

```
Program received signal SIGSEGV, Segmentation fault.
0x00007ffff7c89664 in feof () from /usr/lib/libc.so.6
```

avec la commande `gdb ./dico` on remarque que l'erreur provient d'un appel à la fonction `feof()`.

Après analyse du code, que l'appel à `feof()` qui génère l'erreur est situé dans la fonction `displayDico`.

dans cette fonction, on remarque la ligne suivante:

```c
  f = fopen(DICORES, "rw+");
```
Le mode d'ouverture `rw+` n'est pas valide pour la fonction `fopen()`. Ainsi `f` sera `NULL` et l'appel à `feof(f)` génèrera une erreur de segmentation fault.

Il faudrait remplacer le mode d'ouverture par  `w+`.

Il y a aussi d'autres erreurs :
Le fait que la vérification de l'ouverture du fichier n'est jamais faite pour `DICORES` et `TEXTE`. Ce qui peut aussi générer des erreurs de segmentation fault si les fichiers n'existent pas ou ne peuvent pas être ouverts. Ainsi la vérification de l'ouverture des fichiers devrait être faite après chaque appel à `fopen()`.


Après ces corrections, le programme ne génère plus d'erreur de segmentation fault.

## Modularisation

Nous avons décidé de tout d'abord renommer dico.c en main.c pour distinguer le programme principal du module dico.c qu'on va créer. Le programme est partagé entre 3 modules :
  
## dico.c / dico.h :
  Contient des méthodes/définitions et structures liées à la gestion du dictionnaire.
  
## word.c / word.h :
  Méthodes/définitions liées aux manipulations des mots.
  
## structNoeud.h :
  Toutes les structures qui désignent les attributs du nœud de l'ABR, utilisées par d'autres modules.

Pour le moment, on n'a pas touché les fonctions liées aux fichiers JSON car on n'a pas trouvé 
la bibliothèque de '/opt/json-c-master'.


