/* listCroisieres
   listeDestinations ( nom planete, nbr place)
   listeSouhaitPriorites nom prenom priorite choix1/2 4*6 destinations
   divisé en 2 sous listes : liste_préorganise et liste_libre
   listContraintes
*/



/* @requires enattente non vide, liste_preorganise et liste_libre vide
   @assigns liste_preorganise et liste_libre vide
   @ensures ajoute les personnes qui ont choisi une croisiere organise dans liste_preoganise, de meme avec croisiere libre dans liste_libre
            si il n'y a pas assez de place pour le nombre de personnes, les personnes avec la priorité la plus faible sont exclues
 */
void format_list(listePersonnes enattente, dictDestinations_liste* tableaukiwi, listePersonnes* liste_preorganise, listePersonnes* liste_libre);



/* @requires le nom de la croisiere choisie et les destinations choisies
   @assigns 
   @ensures NULL si le voyage de la premiere personne de la liste courante respecte les contraintes,  sinon renvoie la planete qui ne verifie pas la contrainte
 */
char* verif_contraintes(listePersonnes liste_courante,int choix, dictContraintes contraintes);





/* @requires le nom de la croisiere choisie et les destinations choisies
   @assigns
   @ensures 1 si il reste des places dispo, 0sinon
 */
int verif_quota(listePersonnes liste_courante,int choix, dictDestinations* croisiere_vie,dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre );


/* @requires le nom de la croisiere choisie et les destinations choisies
   @assigns modifie le tableaux des quotas croisiere correspondant à nom_croisiere
   @ensures eneleve une place a chaque planete visitée pendant la croisière
 */
void maj_tab_croisieres(listePersonnes* liste_courante, int choix, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre);


/* @requires
   @assigns resultat, liste_preorganise, liste_libre, liste_unlucky
   @ensures  modifie resultat en rajoutant la personne et l'enleve de liste_preorganise si au moins un de ses 2 choix est possible 
   
   si son 2eme choix est libre il est ajouté à liste_libre (prendre en compte la priorité)
   sinon on l'enleve de liste_préorganise et on la rajoute à liste_unlucky

*/
void affectation_preorganise(listeFinale* resultat,listePersonnes* liste_preorganise,listePersonnes* liste_libre,listePersonnes* liste_unlucky, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre);



/* @requires 
   @assigns resultat,
   @ensures trouve la croisiere la mieux adaptée et rajoute la personne à resultat, met à jour les quotas
 */
void trouve_croisiere(listeFinale* resultat, listePersonnes* liste_libre,dictDestinations_liste* tableaukiwi, dictContraintes contraintes,dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre);


/* @requires 
   @assigns resultat, liste_libre, liste_unlucky
   @ensures (rajoute la personne sur resultat et la supprime de liste_libre) OU (rajoute la personne à liste_unlucky et la supprime de liste_libre
 */
void affectation_libre(listeFinale* resultat,listePersonnes* liste_libre, listePersonnes* liste_unlucky, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre);



/* @requires 
   @assigns resultat, liste_unlucky
   @ensures rajoute la personne sur resultat et la supprime de liste_unlucky
 */
void affectation_unlucky(listeFinale* resultat,listePersonnes* liste_unlucky, dictDestinations_liste* tableaukiwi, dictContraintes contraintes, dictDestinations* croisiere_vie, dictDestinations* croisiere_naturel, dictDestinations* croisiere_planetes, dictDestinations* libre);

