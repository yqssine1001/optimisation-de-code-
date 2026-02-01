#ifndef WORD_H
#define WORD_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define SEP " ,.-"

#define maxSizeWord 25

typedef struct emplacement_t emplacement_t;
struct emplacement_t {
  unsigned int line;
  unsigned int colonne;
  emplacement_t* next;
};

typedef struct mot_data_t mot_data_t;
struct mot_data_t {
  char lemot[maxSizeWord];
  emplacement_t* tete_liste;
  emplacement_t* queue_liste;
};


char *next_word(FILE *f, unsigned int *nblin, unsigned int *nbcol);
int compareWord(mot_data_t* w1, mot_data_t* w2);
void incWord(emplacement_t* location, unsigned int line, unsigned int colonne);
void displayWord(mot_data_t* word, FILE *filedes);


#endif