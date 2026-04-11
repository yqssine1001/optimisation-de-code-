# Rapport Final - MOCA
## Organisation d'un projet

Dès lors qu'un projet est commencé ou repris en cours de route, il est préférable de s'assurer que celui-ci suivent quelques schémas dans sa structure et son organisation afin de permettre son bon développement et/ou sa maintanebilité sur le long terme. 

### Organisation hiéarchique
L'un des premiers outils qui peut être mis en place est l'organisation hiéarchique. Le principe réside en une séparation claire des différents fichiers d'un projet dans différents dossiers à la racine de ce dernier. Par exemple pour le projet qui est utilisé en exemple et qui est rédigé en C, il faut : 
  - Un dossier pour les fichiers sources (`.c`).
  - Un dossier pour les fichiers d'entête (`.h`).
  - Un dossier pour les executable.
  - D'autres dossiers au besoin (documentation, tests, bibliotheque,...).
Ordonner les différentes composantes de cette façon permet une lecture plus aisée du pojet et de qui fait quoi dedans. Cela est aussi pratique pour la compilation avec un Makefile.

### Modularisation du code
Une chose importante, surtout pour maintenir un projet, est la modularisation du code : 
 - Faire d'une fonction ce qui peut être une fonction (factoriser).
 - Découper en différents fichiers.
 - Séparer correctements les différentes composantes entre les fichiers sources.
La conception modulaire a pour but d'organiser le code en différents headers associés (ou non) à un fichier source ainsi qu'un fichier source principal (main) sans header.
#### Exemple
  Sur le projet, il nous était originellement fournit un fichier source contenant l'entièreté des fonctions et structure du projet. Pour respecter la conception modulaire, il a été choisit de découper ce fichier sources en 5 fichiers distinct : 
  - `dico.(c/h)` contenant toutes les méthodes et structures ayant un rapport avec la manipulation d'un dictionnaire de mots (par la suite deviendra une bibliothèque).
  - `word.(c/h)` contenant toutes les méthodes et structures ayant un rapport avec la manipulation des mots.
  - `main.c` contenant la boucle de code principale, il s'agit du code qui fait tourner le programme en utilisant les autres sources. 
  Une fois ce découpage fait, chaque fichier a été placé dans son dossier selon l'organisation hiéarchique.

### Compilation avec un Makefile
Pour compiler un projet, plusieurs choix s'offrent : 
  - Tapper chaque commande à la main (long et innefficace).
  - Utiliser un script `bash` (peut efficace et recompile toujours tout le projet).
  - Utiliser un `Makefile`.
Dès lors qu'un projet utilise une organisation héarchique ainsi qu'une conception modulaire, c'est l'option `Makefile` qui doit être retenue.

Un `Makefile` est un fichier utilisable par l'utilitaire `make` contenant un ensemble de règles et de variables afin de compiler. 

#### Règles
Une règle est instruction appelable avec la commande `make <cible>` qui va effectuer au besoin la règle. Elle se décompose en deux partie : 
1. `cible: condition` sur une première ligne.
2. `commande` sur une seconde ligne avec une indentation (une règle peut effectuer plusieurs commandes si on revient a la ligne avec la même indentation que pour la commande precedente). On peut ajouter un "-" devant la commande pour continuer l'execution malgré une erreur de la commande executé.
La cible est a la fois ce qui est sensé etre produit par la règle et son nom. La condition elle, est ce qui va être vérifié par `make` avant d'executer les commandes de la règle :
 - Si la condition est une règle alors on va appliquer le même principe qu'ici a cette derniere et si elle est executé alors la condition d'execution est remplie.
 - Si la condition est un fichier alors la règle ne sera executé que si sa cible est plus ancienne que le fichier.
 Cette particularité permet de ne pas recompiler l'entièreté d'un projet à chaque fois ce qui peut prendre beaucoup de temps selon la machine ou la taille du dit projet. 
 
 Une règle peut être implicite (comportement de `make`) ou explicite (ecrite par un humain) et l'implicite peut être supprimé lors de l'appel a `make` avec certaines options.
 
On peut construire des règles ne suivant pas le principe de "cible=fichier" comme par exemple une règle supprimant tout les fichiers objets et les binaires générées par la compilation. Ces commandes doivent être ajoutées à une règle du `Makefile` spécifique nommée `.PHONY`.


#### Variables
Une variable en `Makefile` prend plus ou moins le même sens qu'en `shell` : elle stocke du texte.
Il est donc possible de s'en servir pour indiquer le nom des différents répertoires contenants les fichiers nécéssaires à la compilation, de sorte à ce que s'il venait à changer de nom, seul la définition de la variable soit à modifier et de même pour le compilateur et ses flags, les bibliothèques utilisées, ...
Une variable peut être précisée lors de l'appel à `make` par `make cible VARIABLE=VALEUR`.

#### Commandes
En plus des règles et des variables, `make` intègres des commandes tel que `wildcard` permettant de mimer le principe de `ls`. Il y a aussi des equivalent au chercher-remplacer entre les variables pouvant utiliser aussi bien suffixes que pattern.

#### Conditionnel
L'outil `make` propose les mots clefs `if` et `else` permettant d'ajouter a des variables du contenu selon des conditions par exemple des variables definie ou non. Cela permet de compiler avec une gestion plus précise : 
  - Compilation en mode debug, pour avoir le même code qui peut, modulo le temps de compilation, produire instantanément un executable conçu pour déboguer et un produit fini qui peut être partagé le tout sans perdre de temps.
  - Modification de la valeur de certaines macro lors de la compilation ce qui peut permettre d'activer ou de désactiver des fonctionnalitées du code si il est prévu comme tel
  - Changement de compilateur, ...
Les instructions de condition de `make` trouvent leurs équivalent dans les instructions de condition du préprocesseur qui permet donc d'effectuer ce qui est cité plus tôt

#### Exemple
Le `Makefile` du projet illustre l'utilisation de `make` et de ses outils : 
  - Utilisation de règles de pattern et de variables pour la compilation du projet et de son organisation héarchiques ainsi que pour les test.
  - Utilisation de compilation conditionnelle pour un mode debug
  - Utilisation de compilation conditionnelle pour l'activation de fonctionnalitées optionnelles
  - Utilisation de compilation conditionnelle pour l'analyse du projet (aSan, Gcov,...)
  - Règle pour la création d'une documentation, ...
## Bibliothèques

L'intégration de bibliothèques constitue une méthode dans la conception et le développement de projets logiciels.
Une bibliothèque représente un ensemble de modules déjà compilés et testés, qui doit être réutilisables à travers plusieurs projets. Ainsi, de la même manière d'organiser un projet en modules, l'intégration de bibliothèques permet aussi d'améliorer la maintenabilité, et l'organisation du code source.

### Remarquer les modules pouvant être des bibliothèques

Il s'agit d'abord d'identifier les modules candidats à devenir des bibliothèques selon les indicateurs suivant :

- **Indépendance** : Un module qui remplit une fonction isolée des autres composants du projet c'est à dire ne doit pas avoir de liens avec d'autres parties du code (responsabilité unique).

- **Réutilisabilité** : Un module dont les fonctionnalités peuvent être utilisées dans d'autres projets ou contextes, avec une interface bien définie.


#### Exemple

Dans le projet MOCA, nous avons identifié le module `word` qui pouvait être transformé en bibliothèque. En effet, les fonctions de `word.c` sont génériques et indépendantes du reste du projet (la bibliothèque `word` peut servir dans un projet qui nécessite une manipulation de mots, un éditeur de texte par ex.). 


### Types de bibliothèques

Nous avons vu en MOCA deux types principaux de bibliothèques, avec des différences dans leur intégration dans un projet logiciel.

#### Bibliothèques Statiques (.a)

Les **bibliothèques statiques** sont des "archives" contenant du code compilé qui est **copié directement dans l'exécutable final** lors de l'édition de liens.

- Indépendance après compilation : peut être exécuté sur n'importe quel système.
- Augmentation (potentiellement significative) de la taille du binaire final
- Impossibilité de mettre à jour la bibliothèque sans recompiler l'application

#### Bibliothèques Partagées (.so)

Les **bibliothèques partagées** ne sont **pas copiées dans l'exécutable**. L'exécutable contient uniquement des références symboliques, et la bibliothèque est chargée en mémoire à l'exécution depuis le `LIBPATH`.

- Dépendance externe : la bibliothèque doit être disponible au moment de l'exécution
- Taille réduite de l'exécutable (contient uniquement les références)
- Mises à jour faciles de la bibliothèque sans recompiler l'application


Le choix entre les deux types de bibliothèques dépend du contexte du projet : les bibliothèques statiques sont idéales pour la portabilité et l'indépendance, tandis que les bibliothèques dynamiques offrent une meilleure utilisation des ressources et une meilleure maintenance.

Dans le projet MOCA, nous intégrerons la bibliothèque `word` à la fois en version statique et partagée.

### Compilation avec bibliothèques

Pour compiler un projet avec des bibliothèques, il est nécessaire de séparer l'ensemble du code source du projet du code de la bibliothèque. Ainsi, le projet doit être structuré de manière à ce que les fichiers sources et les fichiers d'en-tête des bibliothèques soient organisés dans leurs dossiers spécifiques (ex: `lib_word/` pour la bibliothèque `word`).

Ainsi pour la compilation, en fonction du type de bibliothèque choisi (ou les 2 comme pour le projet), il sera nécessaire d'ajouter les options de compilation appropriées pour les bibliothèques, puis de lier les bibliothèques lors de l'édition de liens. Par exemple, pour notre projet MOCA, `make static` compilera le projet en utilisant la bibliothèque statique `libword.a`, tandis que `make shared` utilisera la bibliothèque partagée `libword.so`.

...

## Analyse de la mémoire

La gestion correcte de la mémoire est primordiale pour la fiabilité d'une application. Les erreurs mémoire (fuites, accès invalides, comportements indéfinis) peuvent causer des crashes, des corruptions de données, des failles de sécurité et ne pas être détectées facilement par des tests fonctionnels classiques.

Pour identifier ces problèmes, plusieurs outils de diagnostic sont proposés dans ce cours.

### Outils de diagnostic mémoire

Dans le projet MOCA, nous avons utilisé trois types d'outils de diagnostic pour détecter différents types d'erreurs mémoire :

#### Valgrind Memcheck

**Valgrind Memcheck** est un émulateur mémoire qui trace les instruction et les accès mémoire sans nécessiter de recompilation.

Il détecte :
- **Fuites mémoire** : allocations dynamiques jamais libérées
- **Accès mémoire tas invalides** : lectures/écritures en dehors des pages allouées
- **Valeurs non initialisées utilisées** : utilisation de données jamais définies
- **Double libération** : appels multiples à `free()` sur le même pointeur

#### AddressSanitizer (ASan)

**AddressSanitizer** est un flag de compilation qui injecte du code de vérification directement dans le binaire (à la différence de Valgrind, ASan nécessite une recompilation avec l'option `-fsanitize=address`).

Il détecte :

- **Accès mémoire tas invalides** (heap buffer overflow, use-after-free) : implique un crash du programme contrairement à Memcheck (envoi d'un signal SIGSEGV).
- **Accès mémoire stack invalides** (stack-buffer-overflow, use-after-return)
- **Fuite mémoire à l'arrêt** : résumé des blocs jamais libérés

#### UndefinedBehaviorSanitizer (UBSan)

**UndefinedBehaviorSanitizer** se concentre uniquement sur les comportements indéfinis du langage.

- Débordements d'entiers signés/non signés
- Divisions par zéro

Le standard C n'imposant pas de comportement défini pour ces cas, ce qui peut entraîner des résultats imprévisibles. UBSan aide à détecter ces situations.

Comme ASan UBSan nécessite une recompilation avec `-fsanitize=undefined`.

### Complémentarité

Ces outils sont complémentaires dans la mesure où ils ciblent des types d'erreurs différents :
- Valgrind est plus exhaustif pour les fuites et les accès invalides.
- ASan indique les accès invalides sur le tas et la pile, avec des traces d'appel précises.
- UBSan se concentre uniquement sur les comportements indéfinis, qui peuvent ne pas être détectés par les autres outils.

il est recommandé d'utiliser ces outils de manière combinée dans un projet logiciel.

Dans le projet MOCA, Asan s'est avéré plus utile pour identifier les accès invalides notamment grace à ses traces d'appel. Cela a permis de cibler rapidement les fautes de gestion de la mémoire afin de les corriger juste après leur identification. Les autres outils (Valgrind, UBSan) ont été utilisés pour confirmer que les corrections effectuées ont résolu les problèmes identifiés.

De la même manière que pour les tests, ces outils de diagnostic nous permet de detecter des erreurs mais pas de prouver leur absence. Il est donc important de les utiliser tout au long du développement pour maintenir la qualité du code et éviter l'accumulation de problèmes mémoire.
