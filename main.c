#include "structures.h"
#include "csvIO.h"
#include "affectation.h"


#include <stdio.h>
#include <stdlib.h>


int main(){

  dictDestinations* croisiere_vie = import_destination_croisiere ("croisiere_vie.csv");
  dictDestinations* croisiere_naturel = import_destination_croisiere ("croisiere_satellites.csv");
  dictDestinations* croisiere_planetes = import_destination_croisiere ("croisiere_planetes.csv");
  dictDestinations* croisiere_libre = import_destination_croisiere ("destinations.csv");
  
  
 
  dictDestinations_liste* tableaukiwi = tabDestinations(croisiere_libre);
  
  dictContraintes contraintes = import_contraintes("contraintes.csv", croisiere_libre);
  	
  dictPriorites priorites = import_priorites("priorites.csv");
	
  listePersonnes listeSouhaits = import_souhaits("souhaits_voyageurs.csv", priorites) ;
	
  	
	
  listePersonnes liste_preorganise = listePersonnes_vide();
  listePersonnes liste_libre = listePersonnes_vide();
  listePersonnes liste_unlucky = listePersonnes_vide();
	
  listeFinale listAdmis = listeFinale_vide();
  listeFinale listTriee = listeFinale_vide();
	
  
  format_list(listeSouhaits, tableaukiwi, &liste_preorganise,  &liste_libre);
  
  
  affectation_preorganise(&listAdmis, &liste_preorganise, &liste_libre, &liste_unlucky, croisiere_vie, croisiere_naturel, croisiere_planetes, croisiere_libre);
  affectation_libre(&listAdmis, &liste_libre,&liste_unlucky, croisiere_vie, croisiere_naturel, croisiere_planetes, croisiere_libre);
  
  listePersonnes_reverse(&liste_unlucky);
  
  affectation_unlucky(&listAdmis, &liste_unlucky,tableaukiwi, contraintes, croisiere_vie, croisiere_naturel, croisiere_planetes, croisiere_libre);

  while (listAdmis != NULL)
    listeFinale_insertionTri(&listTriee, listeFinale_retirer(&listAdmis));
  
  export_liste_finale(listTriee);
	
  return 0;
}
