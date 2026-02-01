# Rapport Hebdomadaire - Semaine 2


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

ainsi, word.h n'inclut plus structNoeud.h, ce qui permet de rendre word.c / word.h indépendant et réutilisable $\implies$ création d'une bibliothèque word :
`lib_word/` :
- `include/word.h` Interface publique de la bibliothèque word
- `src/word.c` La source est gardée car le sujet demande par la suite de pouvoir compiler la bibliothèque
- `lib/` pour les fichiers compilés de la bibliothèque



