#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structures.h"
#include "affectation.h"



struct listePersonnes_base {
  char** tab;
  int priorite;
  listePersonnes next;
};

struct dictDestinations_bucket {
  char* planete;
  int places;
  dictDestinations_liste next;
};

struct listeFinale_base {
  char** tab;
  listeFinale next;
};




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
  }
  /*
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
	int zone = strtol(obligation[2],NULL,10);
	if (strcmp(liste_courante->tab[zone+3],obligation[1])==0){
	    verif=1;
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
	int zone = strtol(obligation[2],NULL,10);
	if (strcmp(liste_courante->tab[zone+9],obligation[1])==0){
	  verif=1;
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
  printf(" \n AFFECTATION PREORGANISE \n\n\n");
  while(*liste_preorganise!=NULL){                    /*Tant que la liste des croisieres preorganisées n'est pas vide*/
    
    if (verif_quota((*liste_preorganise), 1, croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){       /*Si il reste encore des places pour cette croisiere (choix1)*/
	
      maj_tab_croisieres(liste_preorganise, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre);	/*On met a jour les tableau des quotas*/
      printf("Nom Prenom : %14s %14s, choix : %10s affecté\n",(*liste_preorganise)->tab[0],(*liste_preorganise)->tab[1],(*liste_preorganise)->tab[2]); 
      /* dictDestinations_liste l;
      dictDestinations_liste* tableaukiwi2 = tabDestinations(libre);
      l = tableaukiwi2[5];
      while( l!=NULL) {
	printf("(%s %2d) -> ",l->planete, l->places);
	l = l->next;
      }
      printf("\n");*/

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
	  maj_tab_croisieres(liste_preorganise, 2, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre);	/*On met a jour les tableau des quotas*/
	   printf("Nom Prenom : %14s %14s, choix : %10s affecté\n",(*liste_preorganise)->tab[0],(*liste_preorganise)->tab[1],(*liste_preorganise)->tab[3]);
	   /*
	    dictDestinations_liste l;
	    dictDestinations_liste* tableaukiwi2 = tabDestinations(libre);
	    l = tableaukiwi2[5];
	    while( l!=NULL) {
	      printf("(%s %2d) -> ",l->planete, l->places);
	      l = l->next;
	    }
	    printf("\n");*/
	    listeFinale_inserer(resultat, tabFinal(listePersonnes_retirer(liste_preorganise), 2));                          /* on l'ajoute à la liste des admis en le retirant de liste preorganise*/

	    
	   
	    

	    
	}
	else {
	  (*liste_preorganise)->tab[16]="0";
	  (*liste_preorganise)->tab[2]="libre";
	  listePersonnes_inserer(liste_unlucky,listePersonnes_retirer(liste_preorganise));          /* Si ses 2 choix sont impossibles on le met dans la liste unlucky en le retirant de liste preorganise*/
	}
      }	
    }
   
   
  }
}


char** tab_planete_interdite(dictDestinations_liste* tableaukiwi){            /*  [i, planete1, planete2,planete3...] avec i le nombre de planete*/
  int i=0;
  int j;
 
  dictDestinations_liste tmp;
  for(j=0; j<6; j++){
    tmp = tableaukiwi[j];
    while(tmp!=NULL){
      i++;
      tmp=tmp->next;
    }
  }
 
  char** tab;
  tab = (char**) malloc(sizeof(char*)*(i+1));   
  
  for(j=0; j<i+1; j++){
    tab[j]= (char*) calloc(30,sizeof(char));
  }

  tab[0][0]='0';                  /* le nombre de planetes interdites en tout*/
  
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
  
  int i;			
  for(i=4; i<10; i++){   /*Pour chacune des zones on essaye d'avoir le maximum de correspondance possible*/
    if (dictDestinations_rechercher(libre[i-4], (*liste_libre)->tab[i])<=0){   /* Si il n'y a plus de place pour cette planete*/
      int trouv=0;                             /* Permet de savoir si l'on a trouvé une autre planete dispo*/
      dictDestinations_liste tmpkiwi = tableaukiwi[i-4];
      while(trouv==0 && tmpkiwi!=NULL){           /*tant qu'on a pas trouvé de planete et regardé toutes les planetes de la zone*/
	if (dictDestinations_rechercher(libre[i-4], tmpkiwi->planete)>0){                /* si il reste de la place pour cette planete */
	  (*liste_libre)->tab[i]=tmpkiwi->planete;    /*on modfie son itineraire*/
	  trouv=1;
	}
	tmpkiwi=tmpkiwi->next;
      }
    }
  }

  /*On doit verifier si cet itinéraire respect les contraintes*/
  int nb_interdit;
  char * planete1  = verif_contraintes((*liste_libre), 1, contraintes); 
  char** tab_interdit = tab_planete_interdite(tableaukiwi);
  /* printf("planete1 : %s\n",planete1); */

  
  
  
    
  while(planete1!=NULL){  /*Tant que les contraintes ne sont pas vérifiées on modifie l'itinéraire*/
   
    char** info_contrainte = dictContraintes_rechercher(contraintes,planete1); /*  [zone1, planete2, zone2]*/
    
    /* printf("info contrainte p1 z1 p2 z2 : %s %s %s %s\n",planete1, info_contrainte[0],info_contrainte[1], info_contrainte[2]); */
    
    int zone = strtol(info_contrainte[2],NULL,10);
    /* printf("nb de place sur %s : %d\n",info_contrainte[1],dictDestinations_rechercher(libre[zone-1], info_contrainte[1])); */
    
    if(dictDestinations_rechercher(libre[zone-1], info_contrainte[1])>0){                      /*on verifie que la planete2 dans contrainte soit disponible*/
      (*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+3] = info_contrainte[1];    /*on remplace donc la planete qui ne respecte pas la contrainte par la planete2 (verif[2] represente la zone de la planete2)*/

      /*
      printf("Choix : ");
      for(i=4; i<10; i++){
	printf("%s -> ",(*liste_libre)->tab[i]);
	} */
      planete1 = verif_contraintes((*liste_libre), 1, contraintes);
      /* printf("planete1 : %s\n",planete1); */
      
    }
    else {            /*si la planete qui pourrait verifier la contrainte n'est pas dispo, il ne faut pas passer par la planete1 et donc il faut la modifier
			Probleme il faut creer un tableau qui enregistre les planetes interdites qui ne peuvent pas verifier les contraintes. Il faut aussi creer un fonction qui verifie si une planete est dans ce tableau*/
      
     
      tab_interdit[strtol(tab_interdit[0],NULL,10)+1] = planete1;   /*on enregistre la planete1 dans le tableau des planetes interdites*/
      nb_interdit = strtol(tab_interdit[0],NULL,10)+1;
      sprintf(tab_interdit[0],"%d",nb_interdit);         /*on augmente de 1 le nombre de planetes interdites*/

      int trouv=0;        /*permet de savoir si on a trouvé un nouvelle planete qui n'est pas dans le tab des planete interdite*/
      dictDestinations_liste tmp;
      tmp = tableaukiwi[strtol(info_contrainte[0],NULL,10)-1];
      while(trouv==0 && tmp!=NULL){
	/* printf("\nnom planete : %s",tmp->planete); */
	if(tab_appartient(tmp->planete, tab_interdit)==0){ /*si la planete n'est pas interdite*/
	  zone = strtol(info_contrainte[0],NULL,10);
	  if (dictDestinations_rechercher(libre[zone-1],tmp->planete)>0){                                /*on verifie son quota*/
	    (*liste_libre)->tab[strtol(info_contrainte[0],NULL,10)+3]=tmp->planete;   /*on la remplace*/
	    trouv=1;
	  }
	  
	}
	
	tmp=tmp->next;
	
      }
      if (trouv==1){
	/*on est content */
	planete1 = verif_contraintes((*liste_libre), 1, contraintes);/* On verifie les contraintes*/
	/* printf("\nplanete1 = %s\n",planete1); */
      }
      else {  /* Si il est impossible de verifier la contrainte on force l'ajout de la personnes dans resultat, en dépassant le quota */
	char * planete_tmp = (*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+4]; /* exemple Europe */
	(*liste_libre)->tab[strtol(info_contrainte[2],NULL,10)+4] = info_contrainte[1]; /*on remplace donc la planete Europe qui ne respecte pas la contrainte par la planete2=jupiter) */

	listeFinale liste_tmp = (*resultat);
	int modif_effectuee =0;
	while((liste_tmp) != NULL && modif_effectuee==0){             /* tant qu'on a pas trouvé la bonne personne pour effectuer la modification */

	  if(strcmp((liste_tmp)->tab[strtol(info_contrainte[2],NULL,10)+2], info_contrainte[1])==0){ /* si il passe par la planete2   (exemple : jupiter) */
	    if(strcmp((liste_tmp)->tab[strtol(info_contrainte[0],NULL,10)+2],planete1)==0){  /* si il passe par la planete1 (exemple : pluton) */
	      (liste_tmp)=(liste_tmp)->next;  /* on passe au suivant */
	    }
	    else {  /* sinon on effectue un echange de planete entre les 2 personnes */
	      printf("La personne réaffectée est : %s %s \n",(liste_tmp)->tab[0], (liste_tmp)->tab[1]);
 printf("Elle ne passe plus par %s mais par %s\n\n", (liste_tmp)->tab[strtol(info_contrainte[2],NULL,10)+2],planete_tmp);
	      (liste_tmp)->tab[strtol(info_contrainte[2],NULL,10)+2]=planete_tmp;  /* la personne affectée ne passe plus par jupiter mais par Europe */
	     
	int zone = strtol(info_contrainte[2],NULL,10);
	      dictDestinations_incremente(&(libre[zone+-1]), info_contrainte[1]);    /* On rajoute une place pour jupiter */
	      /* printf("\nnb places sur %s : %d\n", info_contrainte[1],dictDestinations_rechercher(libre[strtol(info_contrainte[2],NULL,10)], info_contrainte[1])); */

	      modif_effectuee=1;

	      
	    }
	  }
	  else {

	    (liste_tmp)=(liste_tmp)->next;
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
  printf(" \n\n AFFECTATION LIBRE \n\n\n");
  while((*liste_libre)!=NULL){                    /*Tant que la liste des croisieres libre est non vide*/
    
    if (verif_quota((*liste_libre), 1, croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){       /*Si il reste encore des places pour cette croisiere (choix1)*/
      maj_tab_croisieres(liste_libre, 1, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre); /*On met à jour le tableau des quotas*/ printf("Nom Prenom : %14s %14s, choix : %10s affecté\n",(*liste_libre)->tab[0],(*liste_libre)->tab[1],(*liste_libre)->tab[2]); 
      /*dictDestinations_liste l;
      dictDestinations_liste* tableaukiwi2 = tabDestinations(libre);
      l = tableaukiwi2[5];
      while( l!=NULL) {
	printf("(%s %2d) -> ",l->planete, l->places);
	l = l->next;
      }
      printf("\n");*/
      listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_libre), 1));            /*on l'ajoute à la liste des admis et on le retire de liste_libre*/
          
    }
    
    
    else {  
      if(strcmp((*liste_libre)->tab[3],"libre")!=0){ /* si son choix2 est preorganise */
	if (verif_quota((*liste_libre),2 ,croisiere_vie, croisiere_naturel, croisiere_planetes, libre)==1){  /* on verifie si il rest de la place */
	  maj_tab_croisieres(liste_libre, 2, croisiere_vie, croisiere_naturel,  croisiere_planetes, libre); /*On met à jour le tableau des quotas*/
	  printf("Nom Prenom : %14s %14s, choix : %10s affecté\n",(*liste_libre)->tab[0],(*liste_libre)->tab[1],(*liste_libre)->tab[3]); 
	  /* dictDestinations_liste l;
	  dictDestinations_liste* tableaukiwi2 = tabDestinations(libre);
	  l = tableaukiwi2[5];
	  while( l!=NULL) {
	    printf("(%s %2d) -> ",l->planete, l->places);
	    l = l->next;
	  }
	  printf("\n");*/
	  listeFinale_inserer(resultat,tabFinal(listePersonnes_retirer(liste_libre), 2));            /*on l'ajoute à la liste des admis et on le retire de liste_libre*/
	 
	}
	else {
	  (*liste_libre)->tab[16]="0";
	  (*liste_libre)->tab[2]="libre";
	  listePersonnes_inserer(liste_unlucky, listePersonnes_retirer(liste_libre));
	}
      }
      else {      
	(*liste_libre)->tab[16]="0";
	(*liste_libre)->tab[2]="libre";
	listePersonnes_inserer(liste_unlucky, listePersonnes_retirer(liste_libre));
      }
    }
  }
}
      



void affectation_unlucky(listeFinale* resultat,listePersonnes* liste_unlucky, dictDestinations_liste* tableaukiwi, dictContraintes contraintes, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre){
  printf(" \n\n AFFECTATION UNLUCKY \n\n\n");
  while((*liste_unlucky)!=NULL){                    /*Tant que la liste unlucky est non vide*/
    printf("Nom Prenom : %14s %14s, choix : %10s affecté\n",(*liste_unlucky)->tab[0],(*liste_unlucky)->tab[1],(*liste_unlucky)->tab[2]); 
    trouve_croisiere(resultat,liste_unlucky, tableaukiwi, contraintes, croisiere_vie, croisiere_naturel, croisiere_planetes, libre);
  }
  printf("\n\n NOMBRE DE PLACES RESTANTES \n\n\n");
  dictDestinations_liste l;
  dictDestinations_liste* tableaukiwi2 = tabDestinations(libre);
  int i;
  for (i=0; i<6; i++) {
    l = tableaukiwi2[i];
    printf("ZONE %d",i+1);
    while (l!=NULL) {
      printf("%18s %2d        ",l->planete, l->places);
      l = l->next;
    }
    printf("\n");
  }
  printf("\n\n");
}
