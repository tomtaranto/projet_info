/*
Tables :
Souhait voyageurs + indices de priorité    liste (triée)
Contraintes                                dictionnaire
Destinations                               dictionnaire

priorite
(priorite, [ nom, prenom, choix1, choix2, tableau1, tableau2 ], suivant) -> ()

contraintes
dict (planete -> [zonePlanete,contrainte,zoneContrainte])

destination
[ zone1, zone2, zone3, zone4, zone5, zone6 ]
pour chaque zone : dict (planete -> nbre_places)

fin
([nom, prenom, choix_retenu, planetes_retenues], suivant) -> ()

*/


typedef struct listePersonnes_base* listePersonnes;
typedef struct dictPriorites_base* dictPriorites;

typedef struct dictContraintes_base* dictContraintes;

typedef struct listeFinale_base* listeFinale;

typedef struct dictDestinations_base* dictDestinations;
typedef struct dictDestinations_bucket* dictDestinations_liste;



/* -----------------------------------------------------
                        Personnes
   -----------------------------------------------------
*/

/* @requires
   @assigns
   @ensures retourne une liste vide */
listePersonnes listePersonnes_vide();

/* usage : listePersonnes_inserer( &liste, [nom, prenom, choix1, choix2, planètes du choix1 (6), planètes du choix2 (6), priorité] );

   @requires taille(tableau) = 17
   @assigns listePersonnes
   @ensures insere le tableau dans la liste à la bonne place (liste ordonnée par priorité
            décroissante) */
void listePersonnes_inserer(listePersonnes*, char**);

/* @requires 
   @assigns listePersonnes
   @ensures inverse la liste */
void listePersonnes_reverse(listePersonnes*);

/* @requires
   @assigns listePersonnes
   @ensures retire l'élément en tête de la liste */
char** listePersonnes_retirer(listePersonnes*);





/* @requires
   @assigns
   @ensures retourne un dictionnaire de personnes vide */
dictPriorites dictPriorites_vide(int);

/* usage : dictContraintes_inserer(&dict, [nom,prenom], priorite);

   @requires
   @assigns dictPriorites
   @ensures insere la relation (clef -> valeur) dans le dictionnaire */
void dictPriorites_inserer(dictPriorites*, char**, char*);

/* @requires
   @assigns
   @ensures retourne la valeur associée à la clef  
            renvoie NULL si la clef n'existe pas */
char* dictPriorites_rechercher(dictPriorites, char**);

/* @requires
   @assigns dictPriorites
   @ensures supprime la relation (clef -> valeur) associée à la clef et retourne sa valeur 
            si la clef n'existe pas, renvoie NULL */
char* dictPriorites_supprimer(dictPriorites*, char**);



/* -----------------------------------------------------
                        Contraintes
   -----------------------------------------------------
*/

/* @requires
   @assigns
   @ensures retourne un dictionnaire de contraintes vide */
dictContraintes dictContraintes_vide(int);

/* usage : dictContraintes_inserer(&dict, clef, valeur, dictDestinations);

   @requires
   @assigns dictContraintes
   @ensures insere la relation (clef -> [zoneClef,valeur,zoneValeur]) dans le dictionnaire */
void dictContraintes_inserer(dictContraintes*, char*, char*, dictDestinations*);

/* @requires
   @assigns
   @ensures retourne la valeur associée à la clef
            si la clef n'existe pas, renvoie NULL */
char** dictContraintes_rechercher(dictContraintes, char*);




/* -----------------------------------------------------
                        Destinations
   -----------------------------------------------------
*/

/* @requires n>0
   @assigns
   @ensures retourne un dictionnaire de destinations vide */
dictDestinations dictDestinations_vide(int);

/* usage : dictContraintes_inserer(&dict, destination, nbre_places);

   @requires
   @assigns dictContraintes
   @ensures insere la relation (clef -> valeur) dans le dictionnaire */
void dictDestinations_inserer(dictDestinations*, char*, int);

/* @requires 
   @assigns
   @ensures retourne la valeur associée à la clef, -1 si la clef n'existe pas */
int dictDestinations_rechercher(dictDestinations, char*);

/* @requires
   @assigns
   @ensures decremente de 1 la valeur associée à la clef 
            si la clef n'est pas présente dans le dictionnaire, ne fait rien */
void dictDestinations_decremente(dictDestinations*, char*);


/* @requires
   @assigns
   @ensures incremente de 1 la valeur associée à la clef 
            si la clef n'est pas présente dans le dictionnaire, ne fait rien */
void dictDestinations_incremente(dictDestinations*, char*);


/* @requires taille(tableau) = 6
   @assigns
   @ensures renvoie un tableau de 6 cases où chaque case contient une liste des
            différentes planètes de la zone associée avec le nombre de places  */
dictDestinations_liste* tabDestinations(dictDestinations*);


/* -----------------------------------------------------
                       Choix final
   -----------------------------------------------------
*/

/* @requires
   @assigns
   @ensures retourne un liste vide */
listeFinale listeFinale_vide();

/* @requires taille(tableau) = 17 et le choix = 1 ou 2
   @assigns
   @ensures renvoie un tableau de la forme [nom, prenom, choix retenu, planètes du choix retenu (6)] */
char** tabFinal(char**, int);

/* @requires taille(tableau) = 9
   @assigns listeFinale
   @ensures insère l'élément en tête de liste  */
void listeFinale_inserer(listeFinale*, char**);

/* @requires taille(tableau) = 9
   @assigns listeFinale
   @ensures insère l'élément dans la liste à la bonne place (liste triée par ordre lexicographique
            sur le tuple (nom, prenom) */
void listeFinale_insertionTri(listeFinale*, char**);

/* @requires
   @assigns listeFinal
   @ensures retire l'élément en tête de liste et le retourne
            élément de la forme : [nom, prenom, choix retenu, planètes du choix retenu (6)]*/
char** listeFinale_retirer(listeFinale*);














