#include <stdio.h>
#include <stdlib.h>
#include "dico.h"

#ifndef FINAL

#define FINAL 0
#include "CuTest.h"
#include "AllTests.h"

#endif

int main(int argc, char *argv[]) {
#if FINAL
#ifdef DEBUG
  printf("DEBUG: work in progres!!!\n");
#endif

  if (argc < 2) 
  {
    fprintf(stderr, "Erreur : %s prend au moins un argument <fichier>\n", argv[0]);
    return -1;
  }
  char *texte = argv[1];
  FILE* f = fopen(texte, "r");
  int i;

  if (f == NULL) {
    fprintf(stderr,"Erreur ouverture fichier %s\n",texte);
    return -1;
  }

  unsigned int* line = (unsigned int*) malloc(sizeof(int));
  unsigned int* colonne = (unsigned int*) malloc(sizeof(int));
  char* word = (char*) malloc(sizeof(char)*maxSizeWord);
  dico* dictionary = (dico*) malloc(sizeof(dico));   
  dico* copiedico = NULL;     
  mot_data_t **serialized_dico = (mot_data_t **)malloc(MaxSizeArray*sizeof(mot_data_t *));
  while(!feof(f)) {
    word = next_word(f,line,colonne);
    addToDico(&dictionary,word,line,colonne);
  }
  displayDico(dictionary,texte);
  for(i=0; i<MaxSizeArray; i++)
    serialized_dico[i] = NULL;
  serializeDico(dictionary,serialized_dico);
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      printf("Index %d -> ", i);
      displayWord(serialized_dico[i],stdout);
    }
#ifdef FICJSON
  struct json_object *o;
  o = createJSON(serialized_dico);
  FILE *fic = fopen("monfic.json","w");
  fprintf(fic, "%s\n",
	  json_object_to_json_string_ext(o,JSON_C_TO_STRING_PRETTY));
  fclose(fic);

#endif
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      deserializeDico(&copiedico, serialized_dico[i]);
    }
  printf("A la fin : \n");
  displayNodes(copiedico, stdout);
  fclose(f);

#else
    RunAllTests();
#endif
  return 0;
}
