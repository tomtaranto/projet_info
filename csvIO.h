#include "structures.h"

dictContraintes import_contraintes(char*, dictDestinations*);

dictPriorites import_priorites(char*);

listePersonnes import_souhaits(char*, dictPriorites);

dictDestinations* import_destination_croisiere(char*);

void export_liste_finale(listeFinale);