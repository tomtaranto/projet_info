#include "structures.h"

#include <stdio.h>
#include <stdlib.h>

/* ------------------- dictPriorites ------------------ */
char** maketab(char* prenom, char* nom) {
  char** new = (char**) calloc(2,sizeof(char*));
  new[0] = prenom;
  new[1] = nom;
  return new;
}

/*
void afficherliste(dictPriorites_liste l) {
  while (l != NULL) {
    printf("(%s %s -> %s) -> ",l->clef[0],l->clef[1],l->val);
    l = l->next;
  }
  printf("()\n");
}

void afficherdict(dictPriorites d) {
  int i;
  for (i=0; i<(d->taille); i=i+1) {
    afficherliste(d->tab[i]);
  }
}
*/

/* ------------------- dictDestinations -------------- */
/*
void afficherliste_dest(dictDestinations_liste l) {
  while (l != NULL) {
    printf("(%s -> %d) -> ",l->planete,l->places);
    l = l->next;
  }
  printf("()\n");
}
*/

/* ------------------- dictContraines ---------------- */
/*
void afficherliste_contrainte(dictContraintes_liste l) {
  while (l != NULL) {
    printf("(%s -> %s %s %s) -> ",l->clef, l->val[0], l->val[1], l->val[2]);
    l = l->next;
  }
  printf("()\n");
}
*/


/* ------------------- listeFinale ------------------- */
char** newtab_pers(char* prenom, char* nom, int prio) {
  char** new;
  int i;
  new = (char **) calloc(17,sizeof(char*));
  for (i=0; i<16; i=i+1) new[i] = "rien";
  new[0] = nom;
  new[1] = prenom;
  new[16] = (char*) malloc(3*sizeof(char));
  sprintf(new[16],"%d",prio);
  return new;
}

/*
void afficher_tab(char** tab, int size) {
  int i;
  printf("[");
  for(i=0; i<size-1; i=i+1)
    printf("%s,",tab[i]);
  printf("%s]",tab[size-1]);
}

void afficher_fin(listeFinale pers) {
  while (pers != NULL) {
    afficher_tab(pers->tab,9);
    printf(" -> ");
    pers = pers->next;
  }
  printf("[]\n");
}
*/

int main(void) {
  /*
  dictPriorites d = dictPriorites_vide(1);

  dictPriorites_inserer(&d, maketab("Arya","Stark"), "100");
  dictPriorites_inserer(&d, maketab("Sansa","Stark"), "101");
  dictPriorites_inserer(&d, maketab("Brandon","Stark"), "102");
  dictPriorites_inserer(&d, maketab("Jon","Snow"), "103");
  dictPriorites_inserer(&d, maketab("Tyrion","Lannister"), "104");
  dictPriorites_inserer(&d, maketab("Cersei","Lannister"), "105");
  dictPriorites_inserer(&d, maketab("Barrystan","Selmy"), "106");
  dictPriorites_inserer(&d, maketab("Samwell","Tarly"), "107");
  dictPriorites_inserer(&d, maketab("Ned","Stark"), "108");
  dictPriorites_inserer(&d, maketab("Oberyn","Martell"), "109");
  dictPriorites_inserer(&d, maketab("Robb","Stark"), "110");
  dictPriorites_inserer(&d, maketab("Daenerys","Targaryen"), "111");
  
  afficherdict(d);
  */

 
  dictDestinations d1 = dictDestinations_vide(1);
  dictDestinations d2 = dictDestinations_vide(1);
  dictDestinations d3 = dictDestinations_vide(1);
  dictDestinations d4 = dictDestinations_vide(1);
  dictDestinations d5 = dictDestinations_vide(1);
  dictDestinations d6 = dictDestinations_vide(1);
  
  dictDestinations_inserer(&d1, "Mercure", 50);
  dictDestinations_inserer(&d1, "Terre", 50);
  dictDestinations_inserer(&d2, "Vénus", 50);
  dictDestinations_inserer(&d3, "Jupiter", 50);
  dictDestinations_inserer(&d3, "Neptune", 50);
  dictDestinations_inserer(&d3, "Uranus", 50);
  dictDestinations_inserer(&d4, "Saturne", 50);
  dictDestinations_inserer(&d4, "Pluton", 50);
  dictDestinations_inserer(&d5, "Mars", 50);
  dictDestinations_inserer(&d5, "Soleil", 50);
  dictDestinations_inserer(&d6, "Tatooine", 50);
  /*
  int i;

  printf("Dictionnaire d3 :\n");
  for (i=0; i<d3->taille; i=i+1) {
    afficherliste_dest(d3->tab[i]);
  }
  */
  dictDestinations v[6];
  v[0] = d1; v[1] = d2; v[2] = d3; v[3] = d4; v[4] = d5; v[5] = d6;

  printf("\nContraintes :\n");
  dictContraintes d = dictContraintes_vide(1);
  dictContraintes_inserer(&d,"Soleil","Terre",v);
  dictContraintes_inserer(&d,"Terre","Soleil",v);
  dictContraintes_inserer(&d,"Vénus","Tatooine",v);

  char** tab = dictContraintes_rechercher(d,"Vénus");
  printf("Vénus %s %s %s\n",tab[0],tab[1],tab[2]);

  /*
  for (i=0; i<d->taille; i=i+1) {
    afficherliste_contrainte(d->tab[i]);
  }

  printf("\nRésultat :\n");

  dictDestinations_liste* t;
  t = tabDestinations(v);
  
  for (i=0; i<6; i=i+1) {
    afficherliste_dest(t[i]);
  }
  */

  /*
  listeFinale liste = listeFinale_vide();

  listeFinale_inserer(&liste, tabFinal(newtab_pers("Arya","Stark",942),1));
  listeFinale_inserer(&liste, tabFinal(newtab_pers("Brandon","Stark",200),1));
  listeFinale_inserer(&liste, tabFinal(newtab_pers("Tyrion","Lannister",150),1));
  listeFinale_inserer(&liste, tabFinal(newtab_pers("Ned","Stark",300),1));
  listeFinale_inserer(&liste, tabFinal(newtab_pers("Sansa","Stark",942),1));
  afficher_fin(liste);
  */
  return 0;
}
