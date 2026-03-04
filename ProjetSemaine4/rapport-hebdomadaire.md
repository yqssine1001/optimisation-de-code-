# Rapport Hebdomadaire - Semaine 4

Pour cette semaine, nous avons tester le code, notamment avec des tests systèmes, des tests sur la sérialisation/désérialisation, et des tests unitaires notamment pour la fonction `compareWord` de la bibliothèque word.

Pour cela nous avons ajouté des dossiers dans l'arborescence du projet pour gérer l'implémentation des tests :

- `test_include` : pour les fichiers d'entête des tests
- `test_src` : pour les fichiers sources des tests
- `test_obj` : pour les fichiers objets des tests

Les tests sont lancés par l'exécutable principal du projet `bin/main` crée par la commande `make`.

> Pour lancer le programme principal sans exécuter les tests, **il faut compiler l'application avec la commande `make FINAL=1`**.

## Tests unitaires

Après avoir intégré la bibliothèque de tests unitaires CuTest dans le projet, nous avons créé une suite de tests pour la fonction `compareWord` de la bibliothèque word. Cette suite de tests comprend plusieurs cas de test pour rechercher des défaillances potentielles dans cette dernière :

Etant donné que nous avons connaissance de l'implémentation de `compareWord`, nous avons choisi des cas de test qui couvrent différentes branches de l'implémentation, notamment les tests suivants :

- compareWord(NULL, w2) doit retourner 1 
- compareWord(w1, NULL) doit retourner -1 
- compareWord(NULL, NULL) : w1 est NULL => retourne 1 en premier 
- Deux mots identiques => retourne 0 
- w1 < w2 alphabetiquement ("abc" < "abd") => retourne -1 
- w1 > w2 alphabetiquement ("abd" > "abc") => retourne 1 
- w1 est un prefixe de w2 ("ab" < "abc") => retourne -1 
- w2 est un prefixe de w1 ("abc" > "ab") => retourne 1 
- Casse 'A' < 'a' => retourne -1 
- Mots d'une seule lettre differente "a" et "b" => retourne -1 


Ces 10 tests n'ont pas détecté de défaillance, alors:
- compareWord est spécifiquement correcte.
- Les tests sont insuffisants pour détecter une défaillance potentielle.

## Tests systèmes

Pour les tests systèmes, nous avons créé un répertoire `tests_system` dans lequel deux sous répertoires ont été créés : `input` pour les fichiers d'entrée des tests et `expected_output` pour les résultats attendus.

Les tests systèmes appelent la fonction `monDico` dans un processus fils (pour que les fuites mémoire ou corruptions d'un appel précédent n'affectent pas les tests systèmes suivants), et comparent la sortie de cette dernière avec les résultats attendus.

## Couverture de code

Après avoir ajouté, quelques tests (unitaires et systèmes) automatisés, nous avons utilisé l'outil `gcov` pour analyser si les tests que nous avons écrits couvrent bien le code de notre projet. 

Une des stratégies de test est basée sur la couverture du code : L'idée est que si on exécute pas une partie du code (qui contient une **faute**), alors on ne peut pas produire la défaillance (il est important de noter que la couverture du code n'est pas une garantie absolue de l'absence de défauts).

Pour se donner une idée de la qualité des tests, il suffirait de vérifier la couverture de `src/dico.c` et de `lib_word/src/word.c`.

D'après les résultats de `gcov` :

Pour `dico.c`
```
>>> gcov -b -c -o obj src/dico.c
File 'src/dico.c'
Lines executed:93.91% of 115
Branches executed:100.00% of 48
Taken at least once:87.50% of 48
Calls executed:92.11% of 38
Creating 'dico.c.gcov'

Lines executed:93.91% of 115
```
Pour `word.c`

```
gcov -b -c -o lib_word/obj/ lib_word/src/word.c 
File 'lib_word/src/word.c'
Lines executed:98.51% of 67
Branches executed:100.00% of 58
Taken at least once:89.66% of 58
Calls executed:100.00% of 10
Creating 'word.c.gcov'

Lines executed:98.51% of 67
```

Nous avons une couverture du code de 93.91% pour `dico.c` et de 98.51% pour `word.c`, il reste encore des lignes de code non couvertes par les tests. Il serait intéressant d'analyser ces lignes non couvertes pour voir si elles contiennent des défaillances potentielles et d'ajouter des tests pour les couvrir.
