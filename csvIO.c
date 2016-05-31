#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "csvIO.h"



dictContraintes import_contraintes(char* filename, dictDestinations* tab) {
	
  FILE* fichier = NULL;
  dictContraintes dictionnaire = dictContraintes_vide(5);
  
  /* on réserve la mémoire pour les données issues de la premiere ligne */
  char* valeur1 = (char*) malloc(30*sizeof(char));
  char* valeur2 = (char*) malloc(30*sizeof(char));

  if ( (fichier= fopen(filename,"r")) ) {
    
    /* on parcourt ligne par ligne le fichier en selectionnant les données suivant une disposition caracteristique au .csv: */
    /* les valeurs sont separees par une virgule et la fin est detecte par un '\n' qu'on laisse de cote */
    while (fscanf(fichier, "%[^,],%[^\n]%*c", valeur1, valeur2)==2) {
      
      dictContraintes_inserer(&dictionnaire, valeur1, valeur2, tab);

    /* on realloue la memoire pour la ligne suivante */
      valeur1 = (char*) malloc(30*sizeof(char));
      valeur2 = (char*) malloc(30*sizeof(char));
    }

  }
  else {
    perror("Pb ouverture fichier");
  }

  fclose(fichier);

  return dictionnaire;
}



dictPriorites import_priorites(char* filename) {

  FILE* fichier = NULL;
  dictPriorites dictionnaire = dictPriorites_vide(5);
  char** id = (char**) malloc(2*sizeof(char*));
  
  /* on alloue la memoire pour la premiere ligne */
  id[0]=(char*) malloc(30*sizeof(char));
  id[1]=(char*) malloc(30*sizeof(char));
  char* priorite = (char*) malloc(5*sizeof(char));

  if ( (fichier= fopen(filename,"r")) ) {

    /* on parcourt le fichier .csv ligne par ligne en recuperant les donnees en suivant la disposition caracteristique des fichiers .csv */
    while ( fscanf(fichier, "%[^,],%[^,],%[^\n]%*c", id[0], id[1], priorite)==3) {
      dictPriorites_inserer(&dictionnaire, id, priorite);

      /* on alloue la memoire pour la ligne suivante */
      id = (char**) malloc(2*sizeof(char*));
      id[0]=(char*) malloc(30*sizeof(char));
      id[1]=(char*) malloc(30*sizeof(char));
      priorite = (char*) malloc(5*sizeof(char));
    }
    
  }
  else {
    perror("Pb ouverture fichier");
  }

  fclose(fichier);

  return dictionnaire;
}



listePersonnes import_souhaits(char* filename, dictPriorites dict) {

  FILE* fichier = NULL;
  listePersonnes liste = listePersonnes_vide();
  int i;
  
  /* on reserve la memoire  */
  char** donnees = malloc(28*sizeof(char*));
  for(i=0; i<28; i++){
    donnees[i]= (char*) malloc(30*sizeof(char));
  }
  
    /* le tableau resultat sera construit a l'aide des cases du tableau donnees, il n'est donc pas necessaire d'allouer de la memoire pour ces cases */
  char** resultat = malloc(17*sizeof(char*));

  char** temp = malloc(2*sizeof(char*));
  for(i=0; i<2; i++){
    temp[i]= (char*) malloc(30*sizeof(char));
  }

  if ( (fichier = fopen(filename,"r")) ) {

    /* on récupère les donnees en calquant le format .csv */
    while ( fscanf(fichier,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]%*c",
      donnees[0], donnees[1], donnees[2], donnees[3], donnees[4], donnees[5], donnees[6], donnees[7], donnees[8], donnees[9],
      donnees[10], donnees[11], donnees[12], donnees[13], donnees[14], donnees[15], donnees[16], donnees[17], donnees[18],
      donnees[19], donnees[20], donnees[21], donnees[22], donnees[23], donnees[24], donnees[25], donnees[26], donnees[27])==28) {
      
      /* on cherche à stocker dans la structure, seulement la liste des destinations des croisieres selectionnees, du coup on ne recupere dans resultat que les donnees necessaires */
      for (i=0; i<4; i=i+1)
	resultat[i] = donnees[i];

      if (strcmp(donnees[2],"planetes") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i] = donnees[i];

      else if (strcmp(donnees[2],"satellites") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i] = donnees[i+6];

      else if (strcmp(donnees[2],"vie") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i] = donnees[i+12];

      else if (strcmp(donnees[2],"libre") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i] = donnees[i+18];

      if (strcmp(donnees[3],"planetes") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i+6] = donnees[i];

      else if (strcmp(donnees[3],"satellites") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i+6] = donnees[i+6];

      else if (strcmp(donnees[3],"vie") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i+6] = donnees[i+12];

      else if (strcmp(donnees[3],"libre") == 0)
	for (i=4; i<10; i=i+1)
	  resultat[i+6] = donnees[i+18];
		 
      
      
      temp[0] = donnees[0];
      temp[1] = donnees[1];
      
      /* on recupere la priorite associee a la personne en cours de traitement */
      resultat[16] = dictPriorites_rechercher(dict,temp);
      
      listePersonnes_inserer(&liste, resultat);

    /* on réserve la memoire pour la ligne suivante, ie la prochaine personne */
      donnees = malloc(28*sizeof(char*));
      for(i=0; i<28; i++){
	donnees[i]= (char*) malloc(30*sizeof(char));
      }

      resultat = malloc(17*sizeof(char*));
      
      temp = malloc(2*sizeof(char*));
      for(i=0; i<2; i++){
	temp[i]= (char*) malloc(30*sizeof(char));
      }
    }

  }
  else {
    perror("Pb ouverture fichier");
  }

  fclose(fichier);

  return liste;
}



dictDestinations* import_destination_croisiere(char* filename) {
 
  FILE* fichier = NULL;
  
  /* Allocation de la memoire */
  dictDestinations* tabdictionnaire = (dictDestinations*) calloc(6,sizeof(dictDestinations));
  int i;
  for(i=0; i<6; i++){
    tabdictionnaire[i]= dictDestinations_vide(5);
  }
  char** nom = malloc(6*sizeof(char*));
 
  int* nb = malloc(6*sizeof(int));
  int j;
  int k;
  int l;
  int p;
  

  if ((fichier = fopen(filename,"r"))) {

    /* Allocation memoire */
    char * BUFFER;
    char * TEMP;

    BUFFER = (char*) malloc(255*sizeof(char));
    TEMP = (char*) calloc(30,sizeof(char));
    i=0;
    j=0;
    k=0;
    l=0;
    
    /* on parcourt le fichier ligne par ligne en prenant toute la ligne cette fois-ci */
    while(fscanf(fichier,"%[^\n]%*c",BUFFER)==1) {

        /* on parcourt la ligne caractere par caractere pour palier au probleme des "cases" vides */
      while(BUFFER[i]!='\0') {
        
        /* si on detecte une virgule : soit on en registre TEMP qui contient le dernier element entier dans le tableau nom, soit on l'enregistre dans le tableau de chiffre */
        if (BUFFER[i]==',') {
          /* pour savoir si c'est un nom ou un chiffre sachant que les deux s'alternent de facon reguliere en commencant par un nom */
          if (l%2==0) {
          	nom[l/2]=TEMP;
		/*printf("csvIO: nom[l/2] %s\n",nom[l/2]);*/
		l++;
          }
          else {
          	nb[l/2]=strtol(TEMP,NULL,10);
		/*printf("csvIO: nb[l/2] %d\n",nb[l/2]);*/
		l++;
          }
          
          /* on realloue la memoire pour TEMP afin de recuperer le prochain element */
          TEMP = (char*) calloc(30,sizeof(char));
          j=0;
          i=i+1;
        }
        
        /* si on ne detecte pas de virgule, on continue a ajouter le caractere dans le buffer et on incremente le compteur */
        else {
          TEMP[j] = BUFFER[i];
          i=i+1;
          j=j+1;
        }
      
      }
      
      /* cas du dernier element */
      if (l%2==0) {
          	nom[l/2]=TEMP;
      }
      else {
      	nb[l/2]=strtol(TEMP,NULL,10);
      }
      
      /* on realloue la memoire pour la ligne suivante  */
      TEMP = (char*) calloc(30,sizeof(char));
      /*
      int h;
      for (h=0; h<6; h++)
      printf("csvIO: nom[%d] %s\n",h,nom[h]);*/
      
      /* on vide le buffer */
      for (j=0;j<255;j++) BUFFER[j] = '\0';

        /* on reinitialise les variables */
      i=0;
      j=0;
      l=0;

      for (p=0; p<6; p++)
	if (strcmp(nom[p],"") != 0)
	  dictDestinations_inserer(&(tabdictionnaire[p]),nom[p], nb[p]);
      k=k+1;

      nom = malloc(6*sizeof(char*));
      for (p=0; p<6; p++)
	nom[p] = "";
      nb = malloc(6*sizeof(int));
    }
    free(BUFFER);
  }
  else {
    perror("Pb ouverture fichier");
  }

  fclose(fichier);

  return tabdictionnaire;
}



void export_liste_finale(listeFinale liste) {
  FILE* fichier = NULL;
  char** tab = malloc(9*sizeof(char*));

  if ((fichier = fopen("ListeFinale.csv","w"))) {

    /* on recupere les donnnees et on les met en forme dans un fichier .csv */
    while (liste) {
      tab = listeFinale_retirer(&liste);
      fprintf(fichier, "%s,%s,%s,%s,%s,%s,%s,%s,%s\n",tab[0],tab[1],tab[2],tab[3],tab[4],tab[5],tab[6],tab[7],tab[8]);
    }
  }
  else {
    perror("Pb ouverture de fichier");
  }

  fclose(fichier);
}
