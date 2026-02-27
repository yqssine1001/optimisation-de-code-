#ifndef WORD_H
#define WORD_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \file word.h
 * \brief Fonctions de manipulation des mots
 */

#define SEP " ,.-`~'()<>?!;:\"\\&+*/#@^$=_%[]{}"

#define maxSizeWord 25

typedef struct emplacement_t emplacement_t;
/**
 * @brief Structure de case chainee dans un tableau a deux dimentions
 * @param line la ligne dans le tableau
 * @param colonne la colonne dans le tableau
 * @param next la case suivante (colonne + 1 ou line+1 et colonne <- 0)
 */
struct emplacement_t {
  unsigned int line;
  unsigned int colonne;
  emplacement_t* next;
};

typedef struct mot_data_t mot_data_t;
/**
 * @brief Structure de liste de mots
 * @param tete_liste Premier mot de la liste
 * @param queue_liste Dernier mot de la liste
 */
struct mot_data_t {
  char lemot[maxSizeWord];
  emplacement_t* tete_liste;
  emplacement_t* queue_liste;
};

/**
 * @fn char *next_word(FILE *f, unsigned int *nblin, unsignet int *nbcol)
 * @brief Renvoie le prochain mot du fichier f
 * @param f Le fichier à lire
 * @param nblin le nombre de ligne jusqu'au mot
 * @param nbcol le nombre de colonne du jusqu'au mot
 * @return le contenu du mot lu.
 */
char *next_word(FILE *f, unsigned int *nblin, unsigned int *nbcol);
/**
 * @fn compareWord(mot_data_t *w1, mot_data_t *w2)
 * @brief Compare deux mots entre eux et retourne un entier entre comparaison
 * @param w1 le premier mot a comparer
 * @param w2 le second mot a comparer
 * @return 1 si w1 est nul ou plus grand que w2 et egal a w2, -1 si w2 est nul ou plus grand que w1 et egal à w1, sinon la difference entre les deux mots.
 */
int compareWord(mot_data_t* w1, mot_data_t* w2);
/**
 * @fn incWord(emplacement_t *location, unsigned int line, unsigned int colonne)
 * @brief Ajoute un case d'emplacement en queue
 * @param location la case de depart
 * @param line la ligne pour la nouvelle case
 * @param colonne la colonne pour la nouvelle case
 */
void incWord(emplacement_t* location, unsigned int line, unsigned int colonne);
/**
 * @fn displayWord(mot_data_t *word, FILE *filedes)
 * @brief Affiche un mot dans un fichier ainsi que l'emplacement de chacun de ses caracteres
 * @param word le mot a afficher
 * @param filedes le fichier ou l'ecrire
 */
void displayWord(mot_data_t* word, FILE *filedes);


#endif