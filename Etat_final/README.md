
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

## Analyse/Correction de la mémoire

### Fuite mémoire dans `insertDico()`
**Fichier** : `src/dico.c`

Suppression de l'allocation inutile, `newDictionary` pointe directement sur `*dictionary` sans `malloc`.


### Fuite mémoire dans `displayDico()`

**Fichier** : `src/dico.c`

Suppression du pointeur temporaire alloué inutilement ; appel direct à `displayNodes(dictionary, f)`.


### Fuite mémoire dans `incWord()`

**Fichier** : `lib_word/src/word.c`

Suppression de l'allocation temporaire inutile dans `incWord` ; parcours de la liste directement via le pointeur reçu en argument.

### Valeur non initialisée `location->next` dans `addToDico()`

**Fichier** : `src/dico.c`

Initialisation explicite avec `location->next = NULL` dans `addToDico`.


### Fuites mémoire dans `monDico()`

**Fichier** : `src/dico.c`


`line` et `colonne` sont passés en variables locales (plus de `malloc`), `word` est initialisé à `NULL` puis libéré à chaque itération après `addToDico`.



### Fichier non fermé dans `displayDico()`

**Fichier** : `src/dico.c`


Ajout de `fclose(f)` à la fin de `displayDico`.


### Fuite mémoire dans `deserializeDico()` allocation à chaque récursion

**Fichier** : `src/dico.c`

Déplacement de l'allocation à l'intérieur du bloc conditionnel `if(*dic == NULL)`.


Allocation de `newLinkWord` uniquement lors de l'insertion réelle d'un nouveau noeud, et comparaison avec `elt` sans allocation intermédiaire.


### Fuite mémoire dans `insertDico()` non-libération des doublons

**Fichier** : `src/dico.c`


Dans le cas doublon de `insertDico`, après `incWord`, libération de `linkWord->data.tete_liste` puis de `linkWord`.


### Absence de libération mémoire dans `monDico()`

Des fonctions `freeDico()` et `freeDicoShallow()` ont été ajoutées pour libérer récursivement les arbres binaires et leurs listes chaînées d'emplacements.

`copiedico` partage les listes d'emplacements avec `dictionary` (copie superficielle dans `deserializeDico` depuis `newLinkWord->data = *elt`) d'où l'utilisation de `freeDicoShallow` pour éviter un double `free`.

Ajout de `freeDicoShallow(copiedico)`, `freeDico(dictionary)` et `free(serialized_dico)` en fin de `monDico`, avec `freeDico`/`freeDicoShallow` définies dans `dico.c`.

