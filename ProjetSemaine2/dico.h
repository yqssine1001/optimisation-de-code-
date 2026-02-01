#ifndef DICO_H
#define DICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "structNoeud.h"
#include "word.h"
#ifdef FICJSON
#include "json.h"
#endif

#define MaxSizeArray 16381
#define DICORES "dictionnaires.txt"


typedef struct dico dico;
struct dico {
  mot_t* mot;
  dico *fg, *fd;
};

void insertDico(dico** dictionary, mot_t* linkWord);
void addToDico(dico** dictionary, char* word, unsigned int* line, unsigned int* colonne);
void displayDico(dico* dictionary,char *texte);
void displayNodes(dico *d, FILE* f);
void serializeDico(dico * dictionary, mot_data_t **table);
void deserializeDico(dico** dic, mot_data_t *elt);



#ifdef FICJSON
struct json_object *createJSON(mot_data_t **d);
mot_data_t **extractJSON(struct json_object *o);
#endif


#endif