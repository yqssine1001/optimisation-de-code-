
## Bibliothèques

Conversion du module `word` en bibliothèque.

`word.c` dépendait de structNoeud.h, qui est spécifique au projet. La solution a été de découper structNoeud.h pour récupérer uniquement les parties nécessaires à word pour y mettre dans word.h et de même pour dico.

Ainsi :

- **`lib_word/` :**
  - `lib_word/include/word.h` Interface publique de la bibliothèque word
  - `lib_word/src/word.c` La source pour pouvoir compiler la bibliothèque
  - `lib_word/lib/` pour les fichiers compilés de la bibliothèque


## Compilation avec bibliothèque statique et partagée

Création de deux cibles pour gérer la compilation du projet avec la possibilité de choisir entre une compilation en utilisant les bibliothèques statiques (.a) ou partagées (.so).
- `make static` : compile le projet en utilisant la bibliothèque statique libword.a
- `make shared` : compile le projet en utilisant la bibliothèque partagée libword.so

