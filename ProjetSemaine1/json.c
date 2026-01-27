//#include "json.h"

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