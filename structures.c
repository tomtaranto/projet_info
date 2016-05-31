#include "structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* -----------------------------------------------------
                        Personnes
   -----------------------------------------------------
*/

struct listePersonnes_base {
  char** tab;
  int priorite;
  listePersonnes next;
};

listePersonnes listePersonnes_vide() {
  return NULL;
}

/* @requires
   @assigns
   @ensures renvoie une liste avec l'élément tab en tête de pers */
listePersonnes listePersonnes_cons(listePersonnes pers, char** tab) {
  listePersonnes new;
  new = (listePersonnes) malloc(sizeof(struct listePersonnes_base));
  new->tab = tab;
  new->priorite = strtol(tab[16],NULL,10);
  new->next = pers;
  return new;
} 

void listePersonnes_inserer(listePersonnes* pers, char** tab) {
  if (*pers == NULL)
    (*pers) = listePersonnes_cons(NULL,tab);
  else {
    int priorite = strtol(tab[16],NULL,10);
    if ((*pers)->priorite < priorite)
      (*pers) = listePersonnes_cons(*pers,tab);
    else
      listePersonnes_inserer(&((*pers)->next),tab);
  } 
}

void listePersonnes_reverse(listePersonnes* pers) {
  if (*pers != NULL) {
    listePersonnes old = *pers;
    listePersonnes curr = (*pers)->next;
    listePersonnes next;
    old->next = NULL;
    while (curr != NULL) {
      next = curr->next;
      curr->next = old;
      old = curr;
      curr = next;
    }
    (*pers) = old;
  }
}

char** listePersonnes_retirer(listePersonnes* pers) {
  if ((*pers) == NULL) return NULL;

  listePersonnes tmp = (*pers);
  char** res = (*pers)->tab;
  (*pers) = (*pers)->next;
  free(tmp);
  return res;
}


typedef struct dictPriorites_bucket* dictPriorites_liste;

struct dictPriorites_bucket {
  char** clef;
  char* val;
  dictPriorites_liste next;
};

struct dictPriorites_base {
  dictPriorites_liste* tab;
  int taille;
  int nbelem;
};

/* -------------------------------------------------
                        Hash                        */

/* @requires s>0 nombre d'éléments que contient clef
   @assigns
   @requires renvoie le hash de la clef clef */
int hash(char** clef, int s, int m) {
  int i,j,k;
  k = 0;
  for (i=0; i<s; i=i+1) {
    j = 0;
    while (clef[i][j] != '\0') {
      k = k+(int)clef[i][j];
      j = j+1;
    }
  }
  
  double A = 0.6180339887;
  return m*(k*A - (int)(k*A));
}
/* ------------------------------------------------- */


dictPriorites dictPriorites_vide(int n) {
  dictPriorites new = (dictPriorites) malloc(sizeof(struct dictPriorites_base));
  new->taille = n;
  new->tab = (dictPriorites_liste*) calloc(n,sizeof(dictPriorites_liste));
  new->nbelem = 0;
  return new;
}

/* @requires
   @assigns
   @ensures ajoute la relation (clef->val) en tête de liste */
dictPriorites_liste dictPriorites_cons(dictPriorites_liste l, char** clef, char* val) {
  dictPriorites_liste new = (dictPriorites_liste) malloc(sizeof(struct dictPriorites_bucket));
  new->clef = clef;
  new->val = val;
  new->next = l;
  return new;
}

void dictPriorites_inserer(dictPriorites* d, char** clef, char* val) {
  int h = hash(clef,2,(*d)->taille);
  (*d)->nbelem = (*d)->nbelem+1;
  (*d)->tab[h] = dictPriorites_cons((*d)->tab[h],clef,val);
  if ((*d)->nbelem > (*d)->taille) {
    dictPriorites new = dictPriorites_vide(2*(*d)->taille);
    int i;
    for (i=0; i<(*d)->taille; i=i+1) {
      dictPriorites_liste l = (*d)->tab[i];
      while (l != NULL) {
	dictPriorites_inserer(&new, l->clef, l->val);
	dictPriorites_liste old = l;
	l = l->next;
	free(old);
      }
    }
    free((*d)->tab);
    free(*d);
    (*d) = new;
  }
}

char* dictPriorites_rechercher(dictPriorites d, char** clef) {
  int h = hash(clef,2,d->taille);
  dictPriorites_liste l = d->tab[h];
  while (l != NULL) {
    if ( !strcmp(l->clef[0],clef[0]) && !strcmp(l->clef[1],clef[1]) )
      return l->val;
    l = l->next;
  }
  return NULL;
}
      
char* dictPriorites_supprimer(dictPriorites* d, char** clef) {
  int h = hash(clef,2,(*d)->taille);
  dictPriorites_liste l = (*d)->tab[h];
  
  if (l == NULL) return NULL;

  char* res;
  (*d)->nbelem = (*d)->nbelem-1;
  if ( !strcmp(l->clef[0],clef[0]) && !strcmp(l->clef[1],clef[1]) ) {
    (*d)->tab[h] = (*d)->tab[h]->next;
    res = l->val;
    free(l);
    return res;
  }
  
  dictPriorites_liste old = l;
  dictPriorites_liste curr = l->next;
  while (curr != NULL) {
    if (!strcmp(curr->clef[0],clef[0]) && !strcmp(curr->clef[1],clef[1])) {
      res = curr->val;
      old->next = curr->next;
      free(curr);
      return res;
    }
    else {
      old = curr;
      curr = curr->next;
    }
  }
  return NULL;
}

/* -----------------------------------------------------
                        Destinations
   -----------------------------------------------------
*/

struct dictDestinations_bucket {
  char* planete;
  int places;
  dictDestinations_liste next;
};

struct dictDestinations_base {
  dictDestinations_liste* tab;
  int taille;
  int nbelem;
};

dictDestinations dictDestinations_vide(int n) {
  dictDestinations new = (dictDestinations) malloc(sizeof(struct dictDestinations_base));
  new->taille = n;
  new->tab = (dictDestinations_liste*) calloc(n,sizeof(dictDestinations_liste));
  new->nbelem = 0;
  return new;
}

/* @requires
   @assigns
   @ensures ajoute la relation (clef->val) en tête de liste */
dictDestinations_liste dictDestinations_cons(dictDestinations_liste l, char* clef, int val) {
  dictDestinations_liste new = (dictDestinations_liste) malloc(sizeof(struct dictDestinations_bucket));
  new->planete = clef;
  new->places = val;
  new->next = l;
  return new;
}

void dictDestinations_inserer(dictDestinations* d, char* clef, int val) {
  int h = hash(&clef,1,(*d)->taille);
  (*d)->nbelem = (*d)->nbelem+1;
  (*d)->tab[h] = dictDestinations_cons((*d)->tab[h],clef,val);
  if ((*d)->nbelem > (*d)->taille) {
    dictDestinations new = dictDestinations_vide(2*(*d)->taille);
    int i;
    for (i=0; i<(*d)->taille; i=i+1) {
      dictDestinations_liste l = (*d)->tab[i];
      while (l != NULL) {
	dictDestinations_inserer(&new, l->planete, l->places);
	dictDestinations_liste old = l;
	l = l->next;
	free(old);
      }
    }
    free((*d)->tab);
    free(*d);
    (*d) = new;
  }
}

int dictDestinations_rechercher(dictDestinations d, char* clef) {
  int h = hash(&clef,1,d->taille);
  dictDestinations_liste l = d->tab[h];
  while (l != NULL) {
    if ( !strcmp(l->planete,clef) )
      return l->places;
    l = l->next;
  }
  return -1;
}  

void dictDestinations_decremente(dictDestinations* d, char* clef) {
  int h = hash(&clef,1,(*d)->taille);
  dictDestinations_liste l = (*d)->tab[h];
  while (l != NULL) {
    if ( !strcmp(l->planete,clef) ) {
      l->places = l->places-1;
      return;
    }
    l = l->next;
  }
}


void dictDestinations_incremente(dictDestinations* d, char* clef) {
  int h = hash(&clef,1,(*d)->taille);
  dictDestinations_liste l = (*d)->tab[h];
  while (l != NULL) {
    if ( !strcmp(l->planete,clef) ) {
      l->places = l->places+1;
      return;
    }
    l = l->next;
  }
} 

dictDestinations_liste* tabDestinations(dictDestinations* d) {
  dictDestinations_liste* new = (dictDestinations_liste*) calloc(6,sizeof(dictDestinations_liste));
  dictDestinations_liste l;
  int i,j;
  for (i=0; i<6; i=i+1) {
    for (j=0; j<d[i]->taille; j=j+1) {
      l = d[i]->tab[j];
      while (l != NULL) {
	new[i] = dictDestinations_cons(new[i], l->planete, l->places);

	l = l->next;
      }
    }
   
  }
  return new;
}
 

/* -----------------------------------------------------
                        Contraintes
   -----------------------------------------------------
*/

typedef struct dictContraintes_bucket* dictContraintes_liste;

struct dictContraintes_bucket {
  char* clef;
  char** val;
  dictContraintes_liste next;
};

struct dictContraintes_base {
  dictContraintes_liste* tab;
  int taille;
  int nbelem;
};

dictContraintes dictContraintes_vide(int n) {
  dictContraintes new = (dictContraintes) malloc(sizeof(struct dictContraintes_base));
  new->taille = n;
  new->tab = (dictContraintes_liste*) calloc(n,sizeof(dictContraintes_liste));
  new->nbelem = 0;
  return new;
}

/* @requires taille(tab) = 6
   @assigns
   @ensures renvoie la zone dans laquelle se situe la planete si elle est bien présente
            sinon renvoie -1 */
int zone(dictDestinations* tab, char* planete) {
  int i;
  for (i=0; i<6; i=i+1)
    if (dictDestinations_rechercher(tab[i],planete) != -1)
      return i+1;
  return -1;
}

/* @requires
   @assigns
   @ensures ajoute la relation (clef->val) en tête de liste */
dictContraintes_liste dictContraintes_cons(dictContraintes_liste l, char* clef, char* planete, int zone1, int zone2) {
  dictContraintes_liste new = (dictContraintes_liste) malloc(sizeof(struct dictContraintes_bucket));

  /* création du tableau val */
  char** val = (char**) calloc(3,sizeof(char*));
  
  val[0] = (char*) calloc(2,sizeof(char));
  sprintf(val[0],"%d",zone1);
  val[1] = planete;
  val[2] = (char*) calloc(2,sizeof(char));
  sprintf(val[2],"%d",zone2);

  new->clef = clef;
  new->val = val;
  new->next = l;
  return new;
}

void dictContraintes_inserer(dictContraintes* d, char* clef, char* val, dictDestinations* tab) {
  int h = hash(&clef,1,(*d)->taille);
  (*d)->nbelem = (*d)->nbelem+1;
  (*d)->tab[h] = dictContraintes_cons((*d)->tab[h],clef,val,zone(tab,clef),zone(tab,val));
  if ((*d)->nbelem > (*d)->taille) {
    dictContraintes new = dictContraintes_vide(2*(*d)->taille);
    int i;
    for (i=0; i<(*d)->taille; i=i+1) {
      dictContraintes_liste l = (*d)->tab[i];
      while (l != NULL) {
	dictContraintes_inserer(&new, l->clef, l->val[1], tab);
	dictContraintes_liste old = l;
	l = l->next;
	free(old);
      }
    }
    free((*d)->tab);
    free(*d);
    (*d) = new;
  }
}

char** dictContraintes_rechercher(dictContraintes d, char* clef) {
  int h = hash(&clef,1,d->taille);
  dictContraintes_liste l = d->tab[h];
  while (l != NULL) {
    if ( !strcmp(l->clef,clef) )
      return l->val;
    l = l->next;
  }
  return NULL;
}

     
/* -----------------------------------------------------
                       Choix final
   -----------------------------------------------------
*/

struct listeFinale_base {
  char** tab;
  listeFinale next;
};

listeFinale listeFinale_vide() {
  return NULL;
}

/* @requires
   @assigns
   @ensures renvoie une liste avec l'élément tab en tête de l */
listeFinale listeFinale_cons(listeFinale l, char** tab) {
  listeFinale new;
  new = (listeFinale) malloc(sizeof(struct listeFinale_base));
  new->tab = tab;
  new->next = l;
  return new;
} 

char** tabFinal(char** tab, int choix) {
  char** new = (char**) calloc(9,sizeof(char*));
  int i;
  new[0] = tab[0];
  new[1] = tab[1];
  if (choix == 1) {
    new[2] = tab[2];
    for (i=0; i<6; i=i+1)
      new[3+i] = tab[4+i];
  }
  else {
    new[2] = tab[3];
    for (i=0; i<6; i=i+1)
      new[3+i] = tab[10+i];
  }
  free(tab);
  return new;
}

void listeFinale_inserer(listeFinale* l, char** tab) {
    (*l) = listeFinale_cons(*l,tab);
}

void listeFinale_insertionTri(listeFinale* l, char** tab) {
  if (*l == NULL)
    (*l) = listeFinale_cons(NULL,tab);
  else {
    /* on trie par ordre lexicographique sur le tuple (nom, prenom) */
    if (strcmp(tab[0],(*l)->tab[0]) < 0) /* on trie sur le nom */
      (*l) = listeFinale_cons(*l,tab);
    else if (strcmp(tab[0],(*l)->tab[0]) == 0 && strcmp(tab[1],(*l)->tab[1]) <= 0) /* si les noms sont égaux on trie sur le prenom */
      (*l) = listeFinale_cons(*l,tab);
    else
      listeFinale_insertionTri(&((*l)->next),tab);
  } 
}

char** listeFinale_retirer(listeFinale* l) {
  if ((*l) == NULL) return NULL;

  listeFinale tmp = (*l);
  char** res = (*l)->tab;
  (*l) = (*l)->next;
  free(tmp);
  return res;
}









/* Tests 
#include "csvIO.c"

int main(void) {
  
  dictPriorites dict2;

  dict2 = import_priorites("priorites.csv");
  
  int i;
  int j = 0;
  dictPriorites_liste l;
  for (i=0; i<dict->taille; i=i+1) {
    l = dict->tab[i];
    while (l != NULL) {
      printf("(%s %s -> %s) ", l->clef[0], l->clef[1], l->val);
      j++;
      l = l->next;
    }
    printf("\n");
  }

  return 0;

  listePersonnes list;

  list = import_souhaits("souhaits_voyageurs.csv",dict);
  
  int i;
  while (list != NULL) {
    printf("(");
    for (i=0; i<17; i=i+1)
      printf("%s ",list->tab[i]);
    printf("%d)\n",list->priorite);
    list = list->next;
  }
  
  dictDestinations dict;


  dictDestinations* dtab;
  dtab = import_destination_croisiere("destinations.csv");

  int i;
  int j;
  dictDestinations_liste l;
  for (j=0; j<6; j=j+1) {
    printf("\nZONE %d\n",j);
    dict = dtab[j];
    for (i=0; i<dict->taille; i=i+1) {
      l = dict->tab[i];
      while (l != NULL) {
	printf("(%s %d) ",l->planete,l->places);
	l=l->next;
      }
    }
    printf("\n");
  }

  dictContraintes di;
  di = import_contraintes("contraintes.csv", dtab);

  int i;
  dictContraintes_liste l;
  for (i=0; i<di->taille; i=i+1) {
    l = di->tab[i];
    while (l != NULL) {
      printf("(%s -> %s %s %s) ",l->clef,l->val[0],l->val[1],l->val[2]);
      l=l->next;
    }
    printf("\n");
  }


  
  return 0;
}
*/
      
                     






