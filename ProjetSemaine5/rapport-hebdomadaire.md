# Rapport Hebdomadaire - Semaine 5

## Correction de fuites de mémoires
### Fuites flagrante
Sans passer par un outil comme valgrind, on peut juste en lisant le code voir des fuites de mémoire très visible. 
C'est notamment le cas dans `dico.c` et `word.c`.

- dico.h
    la fonction `insert_dico` contient ces deux lignes : 

     ```c
        dico* tempLocation = (dico*) malloc(sizeof(dico);
        newDictionary = *dictionary;
    ```
      
    Ici, lors de la déclaration de `newDictionary`, la variable est aussi initialisée avec un malloc. Hors, dans la ligne suivante, sa valeur est directement remplacée par le contenu à l'adresse `dictionary`.Cela implique que la mémoire allouée lors de la déclaration n'a pas été libéré et qu'il n'y a plus aucun pointeur vers celle-ci ergo elle a fuité.
- word.h
  la fonction `incWord` contient ces deux lignes :

    ```c
        emplacement_t* tempLocation = (emplacement_t*) malloc(sizeof(emplacement_t);
        tempLocation = location;
    ```
    Ici, le probleme est exactement le même que précédement, une fuite de mémoire bien visible.
