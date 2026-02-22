# Rapport Hebdomadaire - Semaine 4

Pour cette semaine, nous avons tester le code, notamment avec des tests systèmes, des tests sur la sérialisation/désérialisation, et des tests unitaires notamment pour la fonction `compareWord` de la bibliothèque word.

Pour cela nous avons ajouté des dossiers dans l'arborescence du projet pour gérer l'implémentation des tests :

- `test_include` : pour les fichiers d'entête des tests
- `test_src` : pour les fichiers sources des tests
- `test_obj` : pour les fichiers objets des tests

Les tests sont lancés par l'exécutable principal du projet `bin/main`.

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

