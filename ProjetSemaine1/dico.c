#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>

//#include "json.h"

#define maxSizeWord 25
#define SEP " ,.-"   

#define TEXTE "hugo1.txt"
#define DICORES "dictionnaires.txt"

#define MaxSizeArray 16381

char *separators=SEP; 
unsigned int current_line=1;
unsigned int current_col=1; 

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

char *next_word(FILE *f, unsigned int *nblin, unsigned int *nbcol){
  char s[100]; 
  char *res; 
  unsigned int i=0, startl = current_line, startc = current_col;
  char sep;
  sep = fgetc(f);
  while (strchr(separators,sep) != NULL  || sep == '\n') { 
    startc++;
    if (sep == '\n'){ 
      startl++; startc = 1;
    } 
    sep = fgetc(f);
  } 
  ungetc(sep,f);
  if (nblin != NULL) *nblin = startl;  
  if (nbcol != NULL) *nbcol = startc;
  while ((strchr(separators, s[i]=fgetc(f)) == NULL) && s[i] != '\n'){
    i++; startc++;
  }
  sep = s[i]; 
  s[i] = '\0';
  res = (char *)malloc(strlen(s)+1); 
  strcpy(res,s);
  while (strchr(separators,sep) != NULL  || sep == '\n') { 
    startc++;
    if (sep == '\n'){  
      startl++; startc = 1;
    } 
    sep = fgetc(f);
  } 
  ungetc(sep,f);
  current_line = startl;
  current_col = startc;
  return res;
}

typedef struct mot_t mot_t;
struct mot_t {
  int lehash;
  mot_data_t data;
};

typedef struct dico dico;
struct dico {
  mot_t* mot;
  dico *fg, *fd;
};

int compareWord(mot_data_t* w1, mot_data_t* w2) {
  if (w1 == NULL) {
    return 1;
  } else if (w2 == NULL) {
    return -1;
  } else {
    char* word1 = w1->lemot;   
    char* word2 = w2->lemot;
    int minSize = (strlen(word1)<strlen(word2))?strlen(word1):strlen(word2);
    int i= 0;
    int pos = 0;
    while(i<minSize && pos == 0) {
      pos = (word1[i]<word2[i])?-1:(word1[i]>word2[i])?1:0;
      i++;
    }
    return (pos == 0 && strlen(word1) < strlen(word2))?-1:(pos == 0 && strlen(word1) > strlen(word2))?1:pos;
  }
}

/*
struct json_object *createJSON(mot_data_t **d){
  int i;
  json_object *r = json_object_new_object();
  json_object *t = json_object_new_array_ext(MaxSizeArray-1);
  for(i=0; i<MaxSizeArray; i++) {
    if (d[i] != NULL) {
      int j=0;
      struct json_object *newelem = json_object_new_object();
      char* mot = d[i]->lemot;   
      emplacement_t* list = d[i]->tete_liste;
      json_object *l = json_object_new_array();
      json_object_object_add(newelem, "index", json_object_new_int(i));
      json_object_object_add(newelem, "mot", json_object_new_string(mot));
      while(list != NULL) {
	struct json_object *paire = json_object_new_object();
	json_object_object_add(paire,"ligne",json_object_new_int(list->line));
	json_object_object_add(paire,"colonne",json_object_new_int(list->colonne));
	json_object_array_put_idx(l,j,paire);
	j++;
	list = list->next;
      }
      json_object_object_add(newelem, "liste", l);
      json_object_array_add(t,newelem); 
    }
  }
  json_object_object_add(r,"dico",t);
  return r;
}

mot_data_t **extractJSON(struct json_object *o){
  mot_data_t **d = (mot_data_t **)malloc(MaxSizeArray*sizeof(mot_data_t *));
  int k;
  size_t n;
  json_object *t;
  for(k=0; k<MaxSizeArray; k++) d[k] = NULL;
  json_object_object_get_ex(o, "dico", &t); 
  n = json_object_array_length(t);
  for(k=0; k<n; k++) { 
    json_object *i, *m, *l, *line, *col;
    int ind, j;
    size_t n2;
    json_object* structmot = json_object_array_get_idx(t,k);
    json_object_object_get_ex(structmot, "index", &i);  
    ind = json_object_get_int(i);
    d[ind] = (mot_data_t*) malloc(sizeof(mot_data_t));
    json_object_object_get_ex(structmot, "mot", &m);  
    strcpy(d[ind]->lemot,json_object_get_string(m));
    d[ind]->tete_liste = NULL;
    d[ind]->queue_liste = NULL;
    json_object_object_get_ex(structmot, "liste", &l); 
    n2 = json_object_array_length(l);
    for(j=0; j<=n2; j++) { 
      json_object* coord = json_object_array_get_idx(l,j); 
      struct emplacement_t *maillon = (struct emplacement_t *) malloc(sizeof(struct emplacement_t));
      json_object_object_get_ex(coord, "ligne", &line);
      maillon->line = json_object_get_int(line);
      json_object_object_get_ex(coord, "colonne", &col);
      maillon->colonne = json_object_get_int(col);
      maillon->next = NULL;
      if (!d[ind]->tete_liste) {
	d[ind]->tete_liste = d[ind]->queue_liste = maillon;
      }
      else {
	d[ind]->queue_liste->next = maillon;
	d[ind]->queue_liste = maillon;
      }
    }
  }
  return d;
}
*/

void incWord(emplacement_t* location, unsigned int line, unsigned int colonne) {
  emplacement_t* newLocation = (emplacement_t*) malloc(sizeof(emplacement_t));
  emplacement_t* tempLocation = (emplacement_t*) malloc(sizeof(emplacement_t));
  tempLocation = location;
  newLocation->next = NULL;
  newLocation->line = line;
  newLocation->colonne = colonne;
  while(tempLocation->next != NULL) {
    tempLocation = tempLocation->next;
  }
  tempLocation->next = newLocation;
}

int hash(char *m) {
  int l = strlen(m);
  int i, h;
  h = 0;
  for(i=0; i<l; i++)
    h = h + (m[i]*pow(127, l-i-1));
  h = h % MaxSizeArray;
  return h;
}

void insertDico(dico** dictionary, mot_t* linkWord) {
  dico* newDictionary = (dico*) malloc(sizeof(dico));
  dico* newDictionaryPrevious = (dico*) malloc(sizeof(dico));
  newDictionary = *dictionary;
  newDictionaryPrevious = newDictionary; 
  while(newDictionary != NULL && compareWord(&(newDictionary->mot->data),&(linkWord->data))>0) {
    newDictionaryPrevious = newDictionary;
    *dictionary = newDictionary;
    insertDico(&(newDictionary->fg),linkWord);
    return;
  }
  while(newDictionary != NULL && compareWord(&(newDictionary->mot->data),&(linkWord->data))<0) {
    newDictionaryPrevious = newDictionary;
    *dictionary = newDictionary;
    insertDico(&(newDictionary->fd),linkWord);
    return;
  }
  if (newDictionary != NULL && compareWord(&(newDictionary->mot->data),&(linkWord->data))==0) { 
    incWord(newDictionary->mot->data.queue_liste,linkWord->data.tete_liste->line,linkWord->data.tete_liste->colonne);
  }
  else { 
    newDictionary = (dico*) malloc(sizeof(dico));
    newDictionary->mot = linkWord;
    newDictionary->fg = newDictionary->fd = NULL;
    *dictionary = newDictionary;
  }
}

void addToDico(dico** dictionary, char* word, unsigned int* line, unsigned int* colonne) {
  mot_t* newLinkWord = (mot_t*) malloc(sizeof(mot_t));
  emplacement_t* location = (emplacement_t*) malloc(sizeof(emplacement_t));
  strcpy(newLinkWord->data.lemot, word);
  newLinkWord->lehash = hash(word);
  location->line = *line;
  location -> colonne = *colonne;
  newLinkWord->data.tete_liste = location;
  newLinkWord->data.queue_liste = location;
  if(*dictionary==NULL) {
    dico* newDictionary = (dico*) malloc(sizeof(dico));
    newDictionary->mot = newLinkWord;
    newDictionary->fg = newDictionary->fd = NULL;
    *dictionary = newDictionary;
  } else if ((*dictionary)->mot == NULL) {
    (*dictionary)->mot = newLinkWord;
  } else {
    insertDico(dictionary,newLinkWord);
  }
}

void displayWord(mot_data_t* word, FILE *filedes) {
  if (word == NULL) {
  } else {
    char* link = word->lemot;   
    emplacement_t* list = word->tete_liste;
    if (link != NULL) {
      fprintf(filedes,"%s",link);
      fflush(stdout);
    }
    while(list != NULL) {
      fprintf(filedes," (%i,%i)",list->line,list->colonne);
      list = list->next;
    }
  }
}

void deserializeDico(dico** dic, mot_data_t *elt) {
  dico *temp = NULL;
  mot_t* newLinkWord = (mot_t*) malloc(sizeof(mot_t));
  newLinkWord->lehash = hash(elt->lemot);
  newLinkWord->data = *elt;
  if((*dic)==NULL) {
    temp = (dico *)malloc(sizeof(dico));
    temp->fg = temp->fd = NULL;
    temp->mot = newLinkWord;
    *dic = temp;
    return;
  }
  if (compareWord(&((*dic)->mot->data),(mot_data_t*)newLinkWord)>0) {   
    deserializeDico(&(*dic)->fg, elt);
  }
  else
    deserializeDico(&(*dic)->fd, elt);
}

void displayNodes(dico *d, FILE* f) {
  if (d) {
    displayNodes(d->fg, f);
    displayWord(&(d->mot->data),f);
    displayNodes(d->fd, f);
  }
}

void displayDico(dico* dictionary) {
  FILE *f = NULL;
  f = fopen(DICORES, "rw+");
  if (!feof(f))
    printf("Resultat existant dans le fichier resultat, on ecrase\n");
  if (dictionary == NULL) {
    printf("displayDico : NULL\n");
  } else {
    fprintf(f, "Contenu dictionnaire pour %s : \n", TEXTE);
    dico* tempDico = (dico*) malloc(sizeof(dico));
    tempDico = dictionary;
    displayNodes(tempDico, f);
    fflush(f);
  }
}

void serializeDico(dico * dictionary, mot_data_t **table) {
  if (dictionary) {
    serializeDico(dictionary->fg, table);
    table[abs(dictionary->mot->lehash)] = &(dictionary->mot->data); 
    serializeDico(dictionary->fd, table);
  }
}

int main() {
  FILE* f = NULL;
  int i;
  f = fopen(TEXTE, "r");
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
  displayDico(dictionary);
  for(i=0; i<MaxSizeArray; i++)
    serialized_dico[i] = NULL;
  serializeDico(dictionary,serialized_dico);
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      printf("Index %d -> ", i);
      displayWord(serialized_dico[i],stdout);
    }
  /*
  struct json_object *o;
  o = createJSON(serialized_dico);
  FILE *fic = fopen("monfic.json","w");
  fprintf(fic, "%s\n",
	  json_object_to_json_string_ext(o,JSON_C_TO_STRING_PRETTY));
  fclose(fic);
  */
  for(i=0; i<MaxSizeArray; i++)
    if (serialized_dico[i] != NULL) {
      deserializeDico(&copiedico, serialized_dico[i]);
    }
  printf("A la fin : \n");
  displayNodes(copiedico, stdout);
  fclose(f);
  return 0;
}
