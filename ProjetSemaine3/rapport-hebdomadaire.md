# Rapport Hebdomadaire - Semaine 3

## Création de bibliothèques

Au stade actuel du projet, nous avons la stucture suivante :
- main.c : point d'entrée du programme
- dico.c / dico.h : gestion du dictionnaire (chargement, recherche, sérialisation)
- structNoeud.h : définition de la structure de données pour les noeuds du dictionnaire
- word.c / word.h : gestion des mots (création, manipulation)

Parmi les 2 modules du projet, il faut identifier ceux qui peuvent être transformés en bibliothèques, c'est à dire ceux qui offrent des fonctionnalités réutilisables et indépendantes :
- **dico.c / dico.h ?** : Non les fonctions de dico.c semblent spécifiques au projet :
  - Dépendances aux constantes MaxSizeArray, DICORES : Pas de flexibilité pour adapter à d'autres tailles ou formats
- **word.c / word.h ?** : Oui les fonctions de word.c sont plus génériques :
  - Réutilisables : manipulation de mots indépendante du contexte
  - Pas de dépendance forte au projet spécifique
  - Peu susceptibles de changer

Un problème se présente cependant : word.c dépend de structNoeud.h, qui est spécifique au projet. La solution a été de découper structNoeud.h pour récupérer uniquement les parties nécessaires à word pour y mettre dans word.h et de même pour dico.

Ainsi, word.h n'inclut plus structNoeud.h, ce qui permet de rendre word.c / word.h indépendant et réutilisable $\implies$ création d'une bibliothèque word :

- **`lib_word/` :**
  - `lib_word/include/word.h` Interface publique de la bibliothèque word
  - `lib_word/src/word.c` La source est gardée car le sujet demande par la suite de pouvoir compiler la bibliothèque
  - `lib_word/lib/` pour les fichiers compilés de la bibliothèque


## Compilation avec bibliothèque statique et partagée

Après avoir mis les règles de compilation de bibliothèques statiques et partagées dans le Makefile, Nous avons créé deux cibles pour gérer la compilation du projet avec la possibilité de choisir entre une compilation en utilisant les bibliothèques statiques (.a) ou partagées (.so).
- `make static` : compile le projet en utilisant la bibliothèque statique libword.a
- `make shared` : compile le projet en utilisant la bibliothèque partagée libword.so


## Structuration du projet

Etant donné que les fichier (les headers, les sources, les exécutables...) étaient tous regroupés dans le même dossier, pour assurer une meilleure organisation et une meilleure visibilité du projet, nous avons décidé de structurer le projet de la manière suivante :
- `src/` : contient les fichiers sources (.c) du projet
- `include/` : contient les fichiers d'en-tête (.h) du projet
- `lib_word/` : contient les fichiers de la bibliothèque word
- `bin/` : contient les exécutables compilés du projet (juste le main pour l'instant)


## Documentation des fonctions
Afin de mieux comprendre le code, nous avons decide de le documenter. Ainsi, a ete ajoute une "javadoc" a chaque declaration de fonction dans un header, que ce soit les fonctions de la bibliotheque ou celles declarees dans les headers du dossier include. Pour cela, il a ete choisit de respecter l'ecriture des javadoc, pour deux chose :
permettre a un IDE assez intelligent de l'afficher correctement et permettre l'automatisation de l'ecriture d'une vrai documentation externe. Pour ce faire, il y a fallu 
utiliser l'outil doxygen. Ce dernier permet de generer automatiquement une documentation d'un projet en prenant specifiquement certaines extensions de fichier et certains 
formatage de commentaire (pour les fichiers source).
Ainsi, Doxyfile est la configuration de doxygen pour le projet, et genere une sortie en html et en LaTeX necessitant compilation au moyen d'un makefile cree par doxygen.
Pour automatiser cela, la generation de la documentaton se fait a chaque compilation complete du projet (make all) si besoin est et le resultat est stocke dans un dossier
documentation prevu a cet effet. De plus, doxygen pourrait aussi servir pour transformer directement un markdown en pdf.