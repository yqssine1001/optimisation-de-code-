# Rapport Final - MOCA

...

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

