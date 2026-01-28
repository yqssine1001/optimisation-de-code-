#ifndef WORD_H
#define WORD_H
#include <stdio.h>
#include "structNoeud.h"



#define SEP " ,.-"


char *next_word(FILE *f, unsigned int *nblin, unsigned int *nbcol);
int compareWord(mot_data_t* w1, mot_data_t* w2);
void incWord(emplacement_t* location, unsigned int line, unsigned int colonne);
void displayWord(mot_data_t* word, FILE *filedes);


#endif