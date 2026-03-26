# Rapport Hebdomadaire - Semaine 7

## Objectif
L'objectif de cette semaine était de mettre en place un fuzzing pour détecter des comportements indéfinis ou vulnérabilités mémoires dans le projet

## Implementation
-Création des répertoires afl_in(test initial correct) et afl_out (résultats et rapports de crashs)
-Modification du Makefile pour intégrer le compilateur afl-gcc pour permettre à AFL de suivre la couverture de code en temps réel
-Développement de deux scripts de test :
    runAFL.sh : fuzzing standard
    runAFL-Asan.sh : Fuzzing avec l'option ASan activée pour détecter les erreurs de corruption mémoire qui ne provoquent pas de crash immédiat.

## Résultats
Aucun crash n'a été détecté, même en utilisant l'instrumentation ASan. 
Donc soit:
Le programme est assez robuste et ne contient pas des UB.
soit
Le test initial ne permet peut-être pas d'explorer les chemins les plus profonds du code

De plus,AFL indique une exécution lente (*slow exec*). Cela limite le nombre de combinaisons testées par seconde.Il fallait optimiser le programme pour une meilleure performance

