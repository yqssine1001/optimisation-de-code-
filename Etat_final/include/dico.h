#ifndef DICO_H
#define DICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "word.h"
#ifdef FICJSON
#include "json.h"
#endif
/**
 * \file dico.h
 * \brief Fonctions d'utilisation des dictionnaires
 */
#define MaxSizeArray 16381
#define DICORES "dictionnaires.txt"
typedef struct dico dico;
typedef struct mot_t mot_t;
/**
 * @brief Un mot avec son contenu et son hash
 * @param lehash le hash du mot
 * @param data le contenu du mot (emplacement et contenu)
 */
struct mot_t {
  int lehash;
  mot_data_t data;
};
/**
 * @brief Un mot et ses voisins
 * @param mot Le mot contenu
 * @param fg Le mot de gauche
 * @param fd Le mot de droite
 */
struct dico {
  mot_t* mot;
  dico *fg, *fd;
};



/**
 * @fn void insertDico(dico **dictionary, mot_t *linkWord)
 * @brief Ajoute un mot a sa place dans un dictionnaire
 * @param dictionary le dictionaire
 * @param linkWord le mot a ajouter
 * @return null
 */
void insertDico(dico** dictionary, mot_t* linkWord);
/**
 * @fn void insertDico(dico **dictionary, char *word, unsigned int *line, unsigned int *colonne)
 * @brief Ajoute un mot a sa place dans un dictionnaire a partir de ses informations bruts
 * @param dictionary le dictionnaire
 * @param word le contenu du mot a ajouter
 * @param line la ligne ou se situe le mot
 * @param colonne la colonne ou se situe le mot
 */
void addToDico(dico** dictionary, char* word, unsigned int* line, unsigned int* colonne);
/**
 * @fn displayDico(dico *dictionary, char *texte)
 * @brief Affiche le dictionnaire d'un tableau de caractère dans DICORES
 * @param dictionary le dictionnaire a afficher
 * @param texte le texte qui a donne le dictionnaire
 */
void displayDico(dico* dictionary,char *texte);
/**
 * @fn displayNodes(dico *d, FILE *f)
 * @brief affiche une partie d'un dictionnaire dans un fichier spécifique
 * @param d le dictionnaire
 * @param f le fichier où l'afficher
 */
void displayNodes(dico *d, FILE* f);
/**
 * @fn serializeDico(dico *dictionary, mot_dat_ **table)*
 * @brief passe d'un dictionnaire a un tableau de mots
 * @param dictionary le dictionnaire
 * @param table la table pour aplatir
 */
void serializeDico(dico * dictionary, mot_data_t **table);
/**
 * @fn deserializeDico(dico **dic, mot_data_t *elt)
 * @brief passe d'un dictionnaire serialise a un dictionaire
 * @param dic le dictionnaire qui va recevoir le contenu
 * @param elt le dicctionnaire serialise
 */
void deserializeDico(dico** dic, mot_data_t *elt);



#ifdef FICJSON
struct json_object *createJSON(mot_data_t **d);
mot_data_t **extractJSON(struct json_object *o);
#endif

/**
 * @fn int monDico(int argc, char *argv[])
 * @brief Point d'entrée du programme, lance le programme principal
 * @param argc
 * @param argv
 * @return int
 */
int monDico(int argc, char *argv[]);
void freeDico(dico *d);
void displayDicoInternal(dico *dictionary, char *texte, char *filename);
#endif
