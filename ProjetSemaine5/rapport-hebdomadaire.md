# Rapport Hebdomadaire - Semaine 5

Cette semaine, nous avons utilisé les outils de diagnostic mémoire pour identifier et pour corriger les bugs liés à la gestion de la mémoire.

## Mise en place de la méthode de diagnostic

Le Makefile a été modifié pour intégrer l'option `-g` aux `CFLAGS` par défaut, permettant la génération de symboles de débogage pour localisation précise des erreurs (numéros de lignes, noms de fonctions) par les outils.

### Outils

Trois outils ont été manipulé :

- **Valgrind Memcheck** : pour détecter les fuites de mémoire et les accès mémoire invalides avec les options `--leak-check=full --show-leak-kinds=all --track-origins=yes`.
- **AddressSanitizer** : compilation avec `-fsanitize=address` pour détecter les accès mémoire invalides.
- **UndefinedBehaviorSanitizer** : compilation avec `-fsanitize=undefined` pour détecter les comportements indéfinis.

## Diagnostics initiaux

Essai sur `Exemples/hugo.txt` :

### Résultats Valgrind Memcheck


```
HEAP SUMMARY:
==39631==     in use at exit: 241,032 bytes in 2,494 blocks
==39631==   total heap usage: 2,498 allocs, 4 frees, 250,720 bytes allocated
==39631== 
==39631== LEAK SUMMARY:
==39631==    definitely lost: 228,560 bytes in 2,132 blocks
==39631==    indirectly lost: 12,000 bytes in 361 blocks
==39631==      possibly lost: 0 bytes in 0 blocks
==39631==    still reachable: 472 bytes in 1 blocks
==39631==         suppressed: 0 bytes in 0 blocks
```

Les erreurs identifiées :
- Fuites mémoire massives (plus de 228 Ko définitivement perdus).
- Accès à des valeurs non initialisées à 19 reprise (`Conditional jump or move depends on uninitialised value`)

### Résultats AddressSanitizer

```
ERROR: AddressSanitizer: SEGV on unknown address
...
#0 0x55bd867a7c97 in incWord
#1 0x55bd867a66c8 in insertDico src/dico.c:27
#2 0x55bd867a6527 in insertDico src/dico.c:23
#3 0x55bd867a6496 in insertDico src/dico.c:18
...
```

ASan a détecté une erreur : un **SEGV** dans la fonction `incWord`, causé par un accès en lecture via un pointeur non initialisé. L'avantage d'ASan est qu'il a donné une trace d'appel précise, plus lisible que Memcheck pour ce type d'erreur.


### Résultats UndefinedBehaviorSanitizer

UBSan n'a pas détecté d'erreur sur les fichiers de test.

## Bugs identifiés et corrections

### Fuite mémoire dans `insertDico()`
**Fichier** : `src/dico.c`

Un `malloc(sizeof(dico))` était effectué mais le pointeur était immédiatement réassigné à `*dictionary` : bloc alloué étant inaccessible alors il y a fuite mémoire à chaque appel.

***Valgrind Memcheck (blocs " definitely lost " liés à `insertDico`)***.

### Fuite mémoire dans `displayDico()`

**Fichier** : `src/dico.c`

Même motif de bug que précédemment avec un `malloc(sizeof(dico))` réassigné immédiatement.


### Fuite mémoire dans `incWord()`

**Fichier** : `lib_word/src/word.c`

Même motif de bug que précédemment avec un `malloc(sizeof(emplacement_t))` réassigné immédiatement.

### Valeur non initialisée `location->next` dans `addToDico()`

**Fichier** : `src/dico.c`

Lors de la création d'un nouvel `emplacement_t`, le champ `next` n'était jamais initialisé à `NULL`:


***Valgrind (valeur non initialisée)***  
- Des lectures de valeurs non initialisées détectées par Valgrind (`Conditional jump depends on uninitialised value` dans `incWord` à `word.c:74`)

***ASan (SEGV).***
- Un **crash (SEGV)** détecté par ASan, car `incWord` parcourait la liste chaînée en testant `tempLocation->next != NULL` avec une valeur undéfinie.

### Fuites mémoire dans `monDico()`

**Fichier** : `src/dico.c`

`line` et `colonne` étaient alloués sur le tas avec `malloc(sizeof(int))`, des variables locales sur la pile suffisent (éviter de libérer des allocations superflues).

`word` était alloué par `malloc`, puis immédiatement écrasé.

Les mots retournés par `next_word()` sont libérés après utilisation avec `free(word)` dans la boucle.

***Valgrind Memcheck (blocs " definitely lost " liés à `monDico`)***.


### Fichier non fermé dans `displayDico()`

**Fichier** : `src/dico.c`

Le fichier ouvert par `fopen` n'était jamais fermé par `fclose` provoquant une fuite de mémoire.


***Valgrind Memcheck : `still reachable: 472 bytes in 1 blocks`.***

### Fuite mémoire dans `deserializeDico()` allocation à chaque récursion

**Fichier** : `src/dico.c`

Un `mot_t* newLinkWord` était alloué par `malloc` à chaque appel récursif de `deserializeDico`, mais n'était utilisé que lorsque (`*dic == NULL`). Les allocations des appels intermédiaires sont perdues.

On a fait un déplacement de l'allocation à l'intérieur du bloc conditionnel `if(*dic == NULL)`.

***Valgrind Memcheck : blocs " definitely lost " liés à `deserializeDico`***.

### Fuite mémoire dans `insertDico()` non-libération des doublons

**Fichier** : `src/dico.c`

Le `mot_t* linkWord` et son `emplacement_t` passés en argument (alloués par `addToDico`) ne sont pas libérés.

***Valgrind Memcheck : blocs " definitely lost " .

### Absence de libération mémoire dans `monDico()`

Aucune structure (comme dictionnaire, table sérialisée, copie du dictionnaire) n'était libérée en fin de programme.

Des fonctions `freeDico()` et `freeDicoShallow()` ont été ajoutées pour libérer récursivement les arbres binaires et leurs listes chaînées d'emplacements.

`copiedico` partage les listes d'emplacements avec `dictionary` (copie superficielle dans `deserializeDico` depuis `newLinkWord->data = *elt`) d'où l'utilisation de `freeDicoShallow` pour éviter un double `free`.

***Valgrind Memcheck*** : " definitely lost " et " indirectly lost "

## Résultats après correction

### Valgrind Memcheck

```
==50343== HEAP SUMMARY:
==50343==     in use at exit: 0 bytes in 0 blocks
==50343==   total heap usage: 472 allocs, 472 frees, 154,695 bytes allocated
==50343== 
==50343== All heap blocks were freed -- no leaks are possible
==50343== 
==50343== For lists of detected and suppressed errors, rerun with: -s
==50343== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

Valgrind ne détecte plus aucune anomalies.

### AddressSanitizer

Aucune erreur détectée. Le crash SEGV précédent dans `incWord` est résolu.

### UndefinedBehaviorSanitizer

Toujours aucun comportement indéfini détecté.

> Remarque : Le lanceur de test possède des erreurs de mémoire à son exécution, mais le programme du dico lui même n'en présente plus.


## Conclusion

Valgrind et ASan sont complémentaires. Valgrind dans la détection exhaustive des fuites mémoire et des valeurs non initialisées, ASan fournit des messages d'erreur plus exploitables pour les accès invalides (crash immédiat avec contexte plus précis).
