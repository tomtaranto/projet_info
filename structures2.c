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

char** listeFinale_retirer(listeFinale* l) {
  if ((*l) == NULL) return NULL;

  listeFinale tmp = (*l);
  char** res = (*l)->tab;
  (*l) = (*l)->next;
  free(tmp);
  return res;
}







/* @requires enattente non vide, liste_preorganise et liste_libre vide
   @assigns liste_preorganise et liste_libre vide
   @ensures ajoute les personnes qui ont choisi une croisiere organise dans liste_preorganise, de meme avec croisiere libre dans liste_libre
   si il n'y a pas assez de place pour le nombre de personnes, les personnes avec la priorité la plus faible sont exclues
*/
void format_list(listePersonnes enattente, dictDestinations_liste* tableaukiwi, listePersonnes* liste_preorganise, listePersonnes* liste_libre){
  int nbr_place=100000000;                   /* nombre de places disponibles totales */
  int somme=0;
  int compteur=1;
  int i;
  dictDestinations_liste tmp;
  for(i=0; i<6; i++){              /*on parcourt chaque zone */
    tmp=tableaukiwi[i];
    while(tmp!=NULL){    /*tant que la liste n'est pas vide on somme le nombre de place pour chaque planete de la zone*/
      somme+=(tmp)->places;
      tmp=tmp->next;
    }
    if (somme>nbr_place){         /*si la zone contient un nombre de place inferieur à celui d'une zone precedente, on restreint le nombre de places totales */
      nbr_place=somme;
    }
    somme=0;
   
  }
  
  

  while(enattente!=NULL && compteur<=nbr_place){   /*Tant qu'il reste des voyageurs sur la liste enattente et qu'il reste des places disponibles*/
    char ** personne_courante = listePersonnes_retirer(&enattente); /* on prend la premiere personne de la liste (qui a la plus haute priorité)*/
    if (strcmp(personne_courante[2],"libre")!=0){
      listePersonnes_inserer( liste_preorganise, personne_courante);  /* si elle a choisi une croisiere organisée, on l'insere dans liste_preorganise*/
    }
    else  listePersonnes_inserer( liste_libre, personne_courante);    /* si elle a choisi une croisiere libre, on l'insere dans liste_libre*/
    compteur++;
  }/*
   listePersonnes list=*liste_libre;
    if(list==NULL){
      printf("ok");}
  while (list != NULL) {
    printf("(");
    for (i=0; i<17; i=i+1)
      printf("%s ",list->tab[i]);
    printf("%d)\n",list->priorite);
    list = list->next;
    }
  printf("\n\n\n");
  listePersonnes list=*liste_preorganise;
  int compt=0;
  while (list != NULL && compt<15) {
    printf("(");
    compt++;
    for (i=0; i<17; i=i+1)
      printf("%s ",list->tab[i]);
    printf("%d)\n",list->priorite);
    list = list->next;
    
    }*/
}

    
    
      
    
    









/* @requires choix= 1 ou 2
   @assigns 
   @ensures NULL si le voyage de la premiere personne de la liste courante respecte les contraintes, sinon renvoie la planete qui ne verifie pas la contrainte
*/
char* verif_contraintes(listePersonnes liste_courante, int choix, dictContraintes contraintes){
  int verif=0;
   int i;
   if(choix==1){                                               /*Si c'est son choix1 */
    for(i=4; i<10; i++){                                      /*on parcourt les zones */
      verif=0;
      char** obligation = dictContraintes_rechercher(contraintes,liste_courante->tab[i]); /* on verifie si la planete a une contrainte*/
      if (obligation!=NULL){    /*si elle en a une*/
	int j;
	for(j=4; j<10; j++){    /* on verifie que la contrainte est dans la croisiere*/
	  if (strcmp(liste_courante->tab[j],obligation[2])==0){
	    verif=1;
	  }
	}
	if (verif!=1){          /*si la contrainte n'est pas verifiée on renvoie le nom de la planete*/
	  return liste_courante->tab[i];
	}
      }
    }
    return NULL;                  /*Si toutes les contraintes sont verifiées, on renvoie NULL*/
  }
  else {                      /* Si c'est le choix2, on refait pareil*/
    for(i=10; i<16; i++){
      verif=0;
      char** obligation = dictContraintes_rechercher(contraintes,liste_courante->tab[i]);
      if (obligation!=NULL){
	int j;
	for(j=10; j<16; j++){
	  if (strcmp(liste_courante->tab[j],obligation[2])==0){
	    verif=1;
	  }
	}
	if (verif!=1){
	  return liste_courante->tab[i];
	}
      }
    }
    return NULL;
  }
  
  
}
  


    
    
    
      
    






/* @requires choix= 1 ou 2
   @assigns 
   @ensures 1 si il reste des places dispo, 0sinon
*/
int verif_quota(listePersonnes liste_courante,int choix, dictDestinations* croisiere_vie,dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre ){
  
 
  if(strcmp((liste_courante->tab)[choix+1], "libre")==0){         /*Si son choix est libre*/
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	if ( dictDestinations_rechercher(libre[i-4], liste_courante->tab[i])<=0){ /*si il n'y a plus de place on renvoie 0*/
	  return 0;
	}
      }
    }
    else {                                          /* si c'est son choix2*/
      int i;
      for(i=10; i<16; i++){
	if (dictDestinations_rechercher(libre[i-10], liste_courante->tab[i])<=0){
	  return 0;
	}
      }
    }
    return 1;
  }
  if(strcmp(liste_courante->tab[choix+1], "planetes")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	
	if (dictDestinations_rechercher(croisiere_planetes[i-4], liste_courante->tab[i])<=0){
	  return 0;
	}
      }
    }
    else {
      int i;
      for(i=10; i<16; i++){
	if (dictDestinations_rechercher(croisiere_planetes[i-10], liste_courante->tab[i])<=0){
	  return 0;
	}
      }
    }
    return 1;
  }
  
  if(strcmp(liste_courante->tab[choix+1], "satellites")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	if (dictDestinations_rechercher(croisiere_naturel[i-4], liste_courante->tab[i])<=0){
	  
	  return 0;
	}
      }
    }
    else {
      int i;
      for(i=10; i<16; i++){
	if (dictDestinations_rechercher(croisiere_naturel[i-10], liste_courante->tab[i])<=0){
	  return 0;
	}
      }
    }
    return 1;
  }

  if(strcmp(liste_courante->tab[choix+1], "vie")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      
      for(i=4; i<10; i++){
	if (dictDestinations_rechercher(croisiere_vie[i-4], liste_courante->tab[i])<=0){
   
	  return 0;
	  
	}
      }
    }
    else {
      int i;
      for(i=10; i<16; i++){
	if (dictDestinations_rechercher(croisiere_vie[i-10], liste_courante->tab[i])<=0){
	 
	  return 0;
	 
	}
      }
    }
    return 1;
  }
  return 0;
}    
	  
  



/* @requires cette croisiere est disponible, choix= 1 ou 2
   @assigns modifie le tableaux des quotas croisiere correspondant à la croisiere choisie
   @ensures eneleve une place a chaque planete visitée pendant la croisière
*/
void maj_tab_croisieres(listePersonnes* liste_courante, int choix, dictDestinations* croisiere_vie,dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
  if(strcmp((*liste_courante)->tab[choix+1], "libre")==0){         /*Si son choix est libre*/
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	dictDestinations_decremente(&(libre[i-4]), (*liste_courante)->tab[i]);
      }
      return ;
    }
    else {                                          /* si c'est son choix2*/
      int i;
      for(i=10; i<16; i++){
	dictDestinations_decremente(&(libre[i-10]), (*liste_courante)->tab[i]);
      }
      return ;
    }
    
  }
  if(strcmp((*liste_courante)->tab[choix+1], "planetes")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	dictDestinations_decremente(&(croisiere_planetes[i-4]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-4]), (*liste_courante)->tab[i]);
      }
      return ;
    }
    
  
    else {
      int i;
      for(i=10; i<16; i++){
	dictDestinations_decremente(&(croisiere_planetes[i-10]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-10]), (*liste_courante)->tab[i]);
	
      }
      return ;
    }

  }

  if(strcmp((*liste_courante)->tab[choix+1], "satellites")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	dictDestinations_decremente(&(croisiere_naturel[i-4]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-4]), (*liste_courante)->tab[i]);
	
      }
      return ;
      
    }
    else {
      int i;
      for(i=10; i<16; i++){
	dictDestinations_decremente(&(croisiere_naturel[i-10]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-10]), (*liste_courante)->tab[i]);	
      }
      return ;
    }
  }

  if(strcmp((*liste_courante)->tab[choix+1], "vie")==0){
    if(choix==1){                                               /*Si c'est son choix1*/
      int i;
      for(i=4; i<10; i++){
	dictDestinations_decremente(&(croisiere_vie[i-4]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-4]), (*liste_courante)->tab[i]);
	  
      }
      return ;
    }
    else {
      int i;
      for(i=10; i<16; i++){
	dictDestinations_decremente(&(croisiere_vie[i-10]), (*liste_courante)->tab[i]);
	dictDestinations_decremente(&(libre[i-10]), (*liste_courante)->tab[i]);
      }
      return ;
    }
  }
  return ;
}
  
     
     


/* @requires 
   @assigns resultat, liste_préorganise, liste_libre
   @ensures  modifie resultat en rajoutant la personne et l'enleve de liste_preorganise si au moins un de ses 2 choix est possible 
   sinon on l'enleve de liste_préorganise et on la rajoute à la fin de liste_libre
   si son 2eme choix est libre il est ajouté à liste_libre (prendre en compte la priorité)

*/
void affectation_preorganise(listeFinale* resultat,listePersonnes* liste_preorganise,listePersonnes* liste_libre,listePersonnes* liste_unlucky, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
  while(*liste_preorganise!=NULL){                    /*Tant que la liste des croisieres preorganisées n'est pas vide*/
    
    if (verif_quota((*liste_preorganise), 1, croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){       /*Si il reste encore des places pour cette croisiere (choix1)*/
	
      maj_tab_croisieres(liste_preorganise, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre);	/*On met a jour les tableau des quotas*/
      listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_preorganise), 1));            /*on l'ajoute à la liste des admis en le retirant de liste preorganise*/
    }
    else {                                                                    /*Sinon on regarde son 2eme choix*/
     
      if (strcmp((*liste_preorganise)->tab[3],"libre")==0){                    /* Si son deuxieme choix est libre*/
	(*liste_preorganise)->tab[2]=(*liste_preorganise)->tab[3];                /*choix1=choix2*/
	int i;
	for(i=4; i<10; i++){                                                  /*On copie toute les planetes du choix2 dans le choix1*/
	  (*liste_preorganise)->tab[i]=(*liste_preorganise)->tab[i+6];
	}
	listePersonnes_inserer(liste_libre,listePersonnes_retirer(liste_preorganise)); /*on le reinsere dans liste_libre avec sa priorité car maintenant il a un choix libre     */

      }
      else {
	if (verif_quota((*liste_preorganise), 2, croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){   /*Si il reste encore des places pour cette croisiere (choix2)*/
	  maj_tab_croisieres(liste_preorganise, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre);	/*On met a jour les tableau des quotas*/
	    listeFinale_inserer(resultat, tabFinal(listePersonnes_retirer(liste_preorganise), 2));                          /* on l'ajoute à la liste des admis en le retirant de liste preorganise*/
	}
	else {
	  (*liste_preorganise)->tab[16]="0";
	  listePersonnes_inserer(liste_unlucky,listePersonnes_retirer(liste_preorganise));          /* Si ses 2 choix sont impossibles on le met dans la liste unlucky en le retirant de liste preorganise*/
	}
      }	
    }
   
   
  }
}


char** tab_planete_interdite(dictDestinations_liste* tableaukiwi){            /*  [i, planete1, planete2,planete3...] avec i le nombre de planete*/
  int i=0;
  int j;
  printf("fction tab_planete_interdite\n");
  dictDestinations_liste tmp;
  for(j=0; j<6; j++){
    tmp = tableaukiwi[j];
    while(tmp!=NULL){
      i++;
      tmp=tmp->next;
    }
  }
  printf("i : %d\n",i);
  printf("apres boucle for");
  char** tab;
  tab = (char**) malloc(sizeof(char*)*(i+1));   
  
  for(j=0; j<i+1; j++){
    tab[j]= (char*) malloc(30*sizeof(char));
  }

  tab[0]="0";                  /* le nombre de planetes interdites en tout*/
  
  return tab;
}


int tab_appartient(char* str, char** tab){   /*Verifie si str est dans une case du tableau*/
  if(strcmp(tab[0],"0")==0){  /*si tab est vide*/
    return 0;
  }

  int i;
  for(i=1; i<=(strtol(tab[0],NULL,10)+1); i++){
    if(strcmp(str,tab[i])==0){
      return 1;
    }
  }
  return 0;
}


/* @requires 
   @assigns resultat
   @ensures trouve la croisiere la mieux adaptée et rajoute la personne à resultat, la supprime de liste libre, met à jour les quotas
*/
void trouve_croisiere(listeFinale* resultat, listePersonnes* liste_libre,dictDestinations_liste* tableaukiwi, dictContraintes contraintes, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
  printf("debut\n");
  int i;
			
  for(i=4; i<10; i++){   /*Pour chacune des zones on essaye d'avoir le maximum de correspondance possible*/
    if (dictDestinations_rechercher((*libre), (*liste_libre)->tab[i])<=0){   /* Si il n'y a plus de place pour cette planete*/
      int trouv=0;                             /* Permet de savoir si l'on a trouvé une autre planete dispo*/
      while(trouv==0 && tableaukiwi[i-4]!=NULL){           /*tant qu'on a pas trouvé de planete et regardé toutes les planetes de la zone*/
	if ((tableaukiwi[i-4]->places)>0){                /* si il reste de la place pour cette planete */
	  (*liste_libre)->tab[i]=tableaukiwi[i-4]->planete;    /*on modfie son itineraire*/
	  trouv=1;
	}
	tableaukiwi[i-4]=tableaukiwi[i-4]->next;
      }
    }
  }
  printf("fin du for \n");
  /*On doit verifier si cet itinéraire respect les contraintes*/
  int nb_interdit;
  char * planete1  = verif_contraintes((*liste_libre), 1, contraintes); 
  char** tab_interdit = tab_planete_interdite(tableaukiwi);
  while(planete1!=NULL){  /*Tant que les contraintes ne sont pas vérifiées on modifie l'itinéraire*/
   printf("1\n");
    char** info_contrainte = dictContraintes_rechercher(contraintes,planete1); /*  [zone1, planete2, zone2]*/
    if(dictDestinations_rechercher((*libre), info_contrainte[1])>0){                      /*on verifie que la planete2 dans contrainte soit disponible*/
      printf("2\n");
      (*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+3] = info_contrainte[1];    /*on remplace donc la plenete qui ne respecte pas la contrainte par la planete2 (verif[2] represente la zone de la planete2)*/
      planete1 = verif_contraintes((*liste_libre), 1, contraintes);
    }
    else {            /*si la planete qui pourrait verifier la contrainte n'est pas dispo, il ne faut pas passer par la planete1 et donc il faut la modifier
			Probleme il faut creer un tableau qui enregistre les planetes interdites qui ne peuvent pas verifier les contraintes. Il faut aussi creer un fonction qui verifie si une planete est dans ce tableau*/
      printf("3\n");
      printf("%s\n",planete1);
      printf("%s\n",tab_interdit[1]);
      tab_interdit[strtol(tab_interdit[0],NULL,10)+1] = planete1;   /*on enregistre la planete1 dans le tableau des planetes interdites*/
      printf("tab bug");
      nb_interdit = strtol(tab_interdit[0],NULL,10)+1;
      /* à enlever */ tab_interdit[0] = (char *) malloc(30*sizeof(char));
      sprintf(tab_interdit[0],"%d",nb_interdit);         /*on augmente de 1 le nombre de planetes interdites*/

      int trouv=0;        /*permet de savoir si on a trouvé un nouvelle planete qui n'est pas dans le tab des planete interdite*/
      dictDestinations_liste tmp;
      printf("c kiwi qui bug");
      tmp = tableaukiwi[strtol(info_contrainte[0],NULL,10)-1];
      printf("4\n");
      while(trouv==0 && tmp!=NULL){
	if(tab_appartient(tmp->planete, tab_interdit)==0){ /*si la planete n'est pas interdite*/
	  if ((tmp->places)>0){                                /*on verifie son quota*/
	    (*liste_libre)->tab[strtol(info_contrainte[0],NULL,10)+3]=tmp->planete;   /*on la remplace*/
	    trouv=1;
	  }
	  
	}
	
	tmp=tmp->next;
	
      }
      if (trouv==1){
	/*on est content */
	planete1 = verif_contraintes((*liste_libre), 1, contraintes);  /* On verifie les contraintes*/
      }
      else {  /* Si il est impossible de verifier la contrainte on force l'ajout de la personnes dans resultat, en dépassant le quota */
	char * planete_tmp = (*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+4]; /* exemple Europe */
	(*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+4] = info_contrainte[1]; /*on remplace donc la plenete Europe qui ne respecte pas la contrainte par la planete2=jupiter) */

	listeFinale* liste_tmp = resultat;
	int modif_effectuee =0;
	while((*liste_tmp) != NULL && modif_effectuee==0){             /* tant qu'on a pas trouvé la bonne personne pour effectuer la modification */
	  if(strcmp((*liste_tmp)->tab[strtol(info_contrainte[2],NULL,10)+2], info_contrainte[1])==0){ /* si il passe par la planete2   (exemple : jupiter) */
	    if(strcmp((*liste_tmp)->tab[strtol(info_contrainte[0],NULL,10)+2],planete1)==0){  /* si il passe par la planete1 (exemple : pluton) */
	      (*liste_tmp)=(*liste_tmp)->next;  /* on passe au suivant */
	    }
	    else {  /* sinon on effectue un echange de planete entre les 2 personnes */
	      (*liste_tmp)->tab[strtol(info_contrainte[2],NULL,10)+2]=planete_tmp;  /* la personne affectée ne passe plus par jupiter mais par Europe */
	      dictDestinations_incremente(libre, info_contrainte[2]);    /* On rajoute une place pour jupiter */
	      
	    }
	  }
	  else {
	    (*liste_tmp)=(*liste_tmp)->next;
	  }
	}
      
      }
    
    }
  }
  /*On a enfin une croisiere possible*/
  maj_tab_croisieres(liste_libre, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre); /*On met à jour le tableau des quotas*/
  listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_libre), 1)); /*On l'ajoute aux admis en le retirant de liste libre*/
  
  
  
}



  


/* @requires liste des affectations, liste_libre...
   @assigns resultat, liste_libre
   @ensures rajoute la personne sur resultat et la supprime de liste_libre
*/
void affectation_libre(listeFinale* resultat,listePersonnes* liste_libre,listePersonnes* liste_unlucky, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
 
  while((*liste_libre)!=NULL){                    /*Tant que la liste des croisieres libre est non vide*/
    
    if (verif_quota((*liste_libre), 1, croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){       /*Si il reste encore des places pour cette croisiere (choix1)*/
      maj_tab_croisieres(liste_libre, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre); /*On met à jour le tableau des quotas*/
      listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_libre), 1));            /*on l'ajoute à la liste des admis et on le retire de liste_libre*/
    }
    
    
    else {  
      if(strcmp((*liste_libre)->tab[3],"libre")!=0){ /* si son choix2 est preorganise */
	if (verif_quota((*liste_libre),2 ,croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){  /* on verifie si il rest de la place */
	  maj_tab_croisieres(liste_libre, 2, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre); /*On met à jour le tableau des quotas*/
	  listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_libre), 2));            /*on l'ajoute à la liste des admis et on le retire de liste_libre*/
	}
	else {
	  (*liste_libre)->tab[16]="0";
	  listePersonnes_inserer(liste_unlucky, listePersonnes_retirer(liste_libre));
	}
      }
      else {      
	(*liste_libre)->tab[16]="0";
	listePersonnes_inserer(liste_unlucky, listePersonnes_retirer(liste_libre));
      }
    }
  }
}
      



void affectation_unlucky(listeFinale* resultat,listePersonnes* liste_unlucky, dictDestinations_liste* tableaukiwi, dictContraintes contraintes, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
  while((*liste_unlucky)!=NULL){                    /*Tant que la liste unlucky est non vide*/
    trouve_croisiere(resultat,liste_unlucky, tableaukiwi, contraintes, croisiere_vie, croisiere_naturel, croisiere_planetes, libre);
  }
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
      
                     






