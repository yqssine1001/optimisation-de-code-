# Rapport Hebdomadaire - Semaine 9

## Objectif

L’objectif de cette semaine était d’effectuer le profiling du projet afin d'identifier une ou plusieurs fonctions mal optimisées.

-**Modification du Makefile:** Ajout de l’option **-pg** pour permettre à **gpof** de suivre l’exécution du programme. -**Création de fichiers de test volumineux:** Génération de jeux de données conséquents afin d'obtenir des résultats de profiling plus pertinents.

## Résultats

on a trouve la fonction qui est mal optimser

| time % | time cumulative s | time self s | calls | call ms/call | call ms/call | name    |
| ------ | ----------------- | ----------- | ----- | ------------ | ------------ | ------- |
| 97.95  | 26.02             | 26.02       | --    | --           | --           | incWord |

on regardont le code on remarque que incword va rajoute un espace a la fin de la list chaine mais il va prent une localisation dans la list chaine alors il doit parcourir lentiere de la list a chaque foit, elle avais une complexite de O(n)

on la modfier incWord pour que elle prent maintment entete de la list chaine pour accede plus rapidement a la qeueu de la list qui a reduit sa complexite de O(1)
