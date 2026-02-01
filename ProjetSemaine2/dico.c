#include "dico.h"

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
  newDictionary = *dictionary;
  while(newDictionary != NULL && compareWord(&(newDictionary->mot->data),&(linkWord->data))>0) {
    *dictionary = newDictionary;
    insertDico(&(newDictionary->fg),linkWord);
    return;
  }
  while(newDictionary != NULL && compareWord(&(newDictionary->mot->data),&(linkWord->data))<0) {
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

void displayDico(dico* dictionary,char *texte) {
  FILE *f = NULL;
  f = fopen(DICORES, "w+");
  if (f == NULL) {
    fprintf(stderr,"Erreur ouverture fichier %s\n",DICORES);
    return;
  }
  if (!feof(f))
    printf("Resultat existant dans le fichier resultat, on ecrase\n");
  if (dictionary == NULL) {
    printf("displayDico : NULL\n");
  } else {
    fprintf(f, "Contenu dictionnaire pour %s : \n", texte);
    dico* tempDico = (dico*) malloc(sizeof(dico));
    tempDico = dictionary;
    displayNodes(tempDico, f);
    fflush(f);
  }
}

void displayNodes(dico *d, FILE* f) {
  if (d) {
    displayNodes(d->fg, f);
    displayWord(&(d->mot->data),f);
    displayNodes(d->fd, f);
  }
}

void serializeDico(dico * dictionary, mot_data_t **table) {
  if (dictionary) {
    serializeDico(dictionary->fg, table);
    table[abs(dictionary->mot->lehash)] = &(dictionary->mot->data); 
    serializeDico(dictionary->fd, table);
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

// struct json_object *createJSON(mot_data_t **d){
//   int i;
//   struct json_object *r = json_object_new_object();
//   struct json_object *t = json_object_new_array_ext(MaxSizeArray-1);
//   for(i=0; i<MaxSizeArray; i++) {
//     if (d[i] != NULL) {
//       int j=0;
//       struct json_object *newelem = json_object_new_object();
//       char* mot = d[i]->lemot;   
//       emplacement_t* list = d[i]->tete_liste;
//       struct json_object *l = json_object_new_array();
//       json_object_object_add(newelem, "index", json_object_new_int(i));
//       json_object_object_add(newelem, "mot", json_object_new_string(mot));
//       while(list != NULL) {
// 	struct json_object *paire = json_object_new_object();
// 	json_object_object_add(paire,"ligne",json_object_new_int(list->line));
// 	json_object_object_add(paire,"colonne",json_object_new_int(list->colonne));
// 	json_object_array_put_idx(l,j,paire);
// 	j++;
// 	list = list->next;
//       }
//       json_object_object_add(newelem, "liste", l);
//       json_object_array_add(t,newelem); 
//     }
//   }
//   json_object_object_add(r,"dico",t);
//   return r;
// }

// mot_data_t **extractJSON(struct json_object *o){
//   mot_data_t **d = (mot_data_t **)malloc(MaxSizeArray*sizeof(mot_data_t *));
//   int k;
//   size_t n;
//   struct json_object *t;
//   for(k=0; k<MaxSizeArray; k++) d[k] = NULL;
//   json_object_object_get_ex(o, "dico", &t); 
//   n = json_object_array_length(t);
//   for(k=0; k<n; k++) { 
//     struct json_object *i, *m, *l, *line, *col;
//     int ind, j;
//     size_t n2;
//     struct json_object* structmot = json_object_array_get_idx(t,k);
//     json_object_object_get_ex(structmot, "index", &i);  
//     ind = json_object_get_int(i);
//     d[ind] = (mot_data_t*) malloc(sizeof(mot_data_t));
//     json_object_object_get_ex(structmot, "mot", &m);  
//     strcpy(d[ind]->lemot,json_object_get_string(m));
//     d[ind]->tete_liste = NULL;
//     d[ind]->queue_liste = NULL;
//     json_object_object_get_ex(structmot, "liste", &l); 
//     n2 = json_object_array_length(l);
//     for(j=0; j<=n2; j++) { 
//       struct json_object* coord = json_object_array_get_idx(l,j); 
//       struct emplacement_t *maillon = (struct emplacement_t *) malloc(sizeof(struct emplacement_t));
//       json_object_object_get_ex(coord, "ligne", &line);
//       maillon->line = json_object_get_int(line);
//       json_object_object_get_ex(coord, "colonne", &col);
//       maillon->colonne = json_object_get_int(col);
//       maillon->next = NULL;
//       if (!d[ind]->tete_liste) {
// 	d[ind]->tete_liste = d[ind]->queue_liste = maillon;
//       }
//       else {
// 	d[ind]->queue_liste->next = maillon;
// 	d[ind]->queue_liste = maillon;
//       }
//     }
//   }
//   return d;
// }
