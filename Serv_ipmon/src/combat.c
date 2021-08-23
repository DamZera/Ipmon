/*
** combat.c
**
** Système de combat tour par tour.
**
*/

# include "combat.h"
/*void recupererInfoDresseurs () {
	MYSQL_RES *result;
	MYSQL* ipmon_bdd = connect_bdd ();
	MYSQL_ROW row;  
	unsigned int num_champs = 0;

	/*On ajoute le pseudo et le pass dans la requete*//*
	char *requete = "SELECT * FROM ipdex, dresseur, ipmon WHERE `ipdex`.dresseur = `dresseur`.Id_Dresseur AND `ipdex`.ipmon = `ipmon`.Id_IPmon";
	if(mysql_query(ipmon_bdd, requete)){
    	finish_with_error(ipmon_bdd);
	}

	result = mysql_store_result(ipmon_bdd);
	num_champs = mysql_num_fields(result);
	while ((row = mysql_fetch_row(result))){
		unsigned long *lengths;
		lengths = mysql_fetch_lengths(result);
		int i;
		for(i = 0; i < num_champs; i++) {
           //On ecrit toutes les valeurs
           printf("%s ", row[i]);
           if (row[0] == row[2]) {
           		Dresseur dresseur;
           }
        }
        printf("\n");
	}

	//Libération du jeu de résultat
    mysql_free_result(result);

    //Fermeture de MySQL
    mysql_close(&ipmon_bdd);
}*/

int chercherDresseur (int id) {
	int i;

	for (i = 0; i < 100; i++) {
		if (dresseurs[i].id == id)
			return 0;
	}
	return -1;
}

Ipmon chercherIpmonDeDresseur (Dresseur dresseur, int id) { //Recherche un ipmon parmi tous les ipmons d'un dresseur
	Ipmon ipmon;
	ipmon.id = -1;
	int i;

	for (i = 0; i < dresseur.nombreIpmons; i++) {
		if (dresseur.ipmons[i].id == id)
			ipmon = dresseur.ipmons[i];
	}
	return ipmon;
}

int chercherIpmonIndiceDeDresseur (Dresseur dresseur, int id) {
	int ipmonIndice = -1;
	int i;

	for (i = 0; i < dresseur.nombreIpmons; i++) {
		if (dresseur.ipmons[i].id == id)
			ipmonIndice = i;
	}
	return ipmonIndice;
}

void EchangerIpmons (Dresseur *dresseur1, Dresseur *dresseur2, Ipmon ipmonDresseur1, Ipmon ipmonDresseur2) {
	int indice1 = chercherIpmonIndiceDeDresseur (*dresseur1, ipmonDresseur1.id);
	int indice2 = chercherIpmonIndiceDeDresseur (*dresseur2, ipmonDresseur2.id);

	Dresseur *dresseur;
	Ipmon ipmon;

	if ((indice1 != -1) && (indice2 != -1)) {
		ipmon = dresseur1 -> ipmons[indice1];
		dresseur1 -> ipmons[indice1] = dresseur2 -> ipmons[indice2];
		dresseur2 -> ipmons[indice2] = ipmon;
	}
}


Dresseur* Ajouter_Ipmon_Dresseur (Dresseur *dresseur, Ipmon ipmon) {
	dresseur -> nombreIpmons ++;
	dresseur -> ipmons [dresseur -> nombreIpmons] = ipmon;

	return dresseur;
}

int CombatDresseurIpmon (Dresseur dresseur, Ipmon ipmon) { //Retourne 1 si dresseur gagne, 2 si ipmon sauvage gagne, 0 si match nul et 3 si dresseur abandonne
	int gagnant;
	Ipmon ipmonDresseur;
	int id, i;

	printf("**************************************\n");
	printf("Ipmon sauvage : \n\tId : %d\n\tNom : %s\n\tEtat : %s\n\tType : %s\n\tPoint de vie : %d\n\tAgilité : %d\n\tNiveau : %d\n\tPuissance attaque : %d\n\tNom attaque : %s\n\tPuissance défense : %d\n\tNom défense : %s\n\t", ipmon.id, ipmon.nom, ipmon.etat, ipmon.type, ipmon.pv, ipmon.agilite, ipmon.niveau, ipmon.puissance_attaque, ipmon.nom_attaque, ipmon.puissance_defense, ipmon.nom_defense);
	printf("Dresseur : \n\tPseudo : %s\n\tNiveau : %d\n", dresseur.pseudo, dresseur.niveau);

	printf("Liste de tous les ipmons du dresseur\n");
	for (i = 0; i < dresseur.nombreIpmons; i++) {
		printf("%s, id = %d\n", dresseur.ipmons[i].nom, dresseur.ipmons[i].id);
	}

	do {	
		printf("Entrez l'id du pokemon choisi pour ce combat\n");
		scanf ("%d", &id);
		ipmonDresseur = chercherIpmonDeDresseur (dresseur, id);
	}while (ipmonDresseur.id == -1);

	printf("Vous avez choisi l'ipmon : %s\n", ipmonDresseur.nom);

	srand (time (NULL));

	int tour = 1, choix;

	if (ipmonDresseur.agilite > ipmon.agilite)
		printf("Le dresseur commence le jeu\n");
	else
		printf("L'ipmon commence le jeu\n");

	//Debut du combat
	while ((ipmonDresseur.pv > 0) && (ipmon.pv > 0)) { //Tant que l'un des ipmons ne meurt pas
		printf("Tour %d\n", tour);
		if (ipmonDresseur.agilite > ipmon.agilite) { //L'ipmon du dresseur est plus rapide que l'ipmon sauvage, donc c'est lui qui attaque en 1er
			printf("Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n");
			scanf ("%d", &choix);
			if (choix == 1) {
				int x = ipmon.pv, y = ipmonDresseur.pv;
				ipmon.pv = attaque (&ipmonDresseur, &ipmon);
				printf("Vous avez fait l'attaque : %s\n", ipmonDresseur.nom_attaque);
				if (x == ipmon.pv)
					printf("Attaque ratée !\n");
				else {
					if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
						gagnant = 1;
						break;
					}
					printf("Il lui reste maintenant %d pv\n", ipmon.pv);
				}
				int a = rand () % 2;
				if (a == 0) {
					printf("L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
					ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
					if (y == ipmonDresseur.pv)
						printf("L'attaque de l'ipmon sauvage est ratée !\n");
					else {
						if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
					}
				}
				else {
					printf("L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque_special);
					ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
					if (y == ipmonDresseur.pv)
						printf("L'attaque spéciale de l'ipmon sauvage est ratée !\n");
					else {
						if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
					}
				}
			}
			else if (choix == 2) {
				int x = ipmon.pv, y = ipmonDresseur.pv;
				ipmon.pv = attaque_speciale (&ipmonDresseur, &ipmon);
				printf("Vous avez fait l'attaque spéciale : %s\n", ipmonDresseur.nom_attaque_special);
				if (x == ipmon.pv)
					printf("Attaque ratée !\n");
				else {
					if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
						gagnant = 1;
						break;
					}
					printf("Il lui reste maintenant %d pv\n", ipmon.pv);
				}
				int a = rand () % 2;
				if (a == 0) {
					printf("L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque_special);
					ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
					if (y == ipmonDresseur.pv)
						printf("L'attaque spéciale de l'ipmon sauvage est ratée !\n");
					else {
						if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
					}
				}
				else {
					printf("L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
					ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
					if (y == ipmonDresseur.pv)
						printf("L'attaque de l'ipmon sauvage est ratée !\n");
					else {
						if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
					}
				}
			}
			else if (choix == 0) {
				printf("Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? y/n\n");
				char c;
				viderBuffer ();
				scanf("%c", &c);
				if (c == 'y') {
					printf("Vous avez abandonné le combat, vous avez donc perdu\n");
					gagnant = 3;
					break;
				}
				
			}
			else {
				printf ("Entrez un nombre valide !");
			}
		}
		else {
			int attaque_ipmon_sauvage = rand () % 2; //0 = attaque simple, 1 = Attaque spéciale
			int x = ipmon.pv, y = ipmonDresseur.pv;
			if (attaque_ipmon_sauvage == 0) {
				printf("L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
				ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
				if (y == ipmonDresseur.pv)
					printf("L'attaque de l'ipmon sauvage est ratée !\n");
				else {
					if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
						gagnant = 2;
						break;
					}
					printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
				}
			}
			else {
				printf("L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque);
				ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
				if (y == ipmonDresseur.pv)
					printf("L'attaque spéciale de l'ipmon sauvage est ratée !\n");
				else {
					if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
						gagnant = 2;
						break;
					}
					printf("Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
				}
			}

			printf("Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n");
			scanf ("%d", &choix);
			if (choix == 1) {
				ipmon.pv = attaque (&ipmonDresseur, &ipmon);
				printf("Vous avez fait l'attaque : %s\n", ipmonDresseur.nom_attaque);
				if (x == ipmon.pv)
					printf("Attaque ratée !\n");
				else {
					if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
						gagnant = 1;
						break;
					}
					printf("Il lui reste maintenant %d pv\n", ipmon.pv);
				}
			}
			else if (choix == 2) {
				ipmon.pv = attaque_speciale (&ipmonDresseur, &ipmon);
				printf("Vous avez fait l'attaque spéciale : %s\n", ipmonDresseur.nom_attaque);
				if (x == ipmon.pv)
					printf("Attaque ratée !\n");
				else {
					if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
						gagnant = 1;
						break;
					}
					printf("Il lui reste maintenant %d pv\n", ipmon.pv);
				}
			}
			else if (choix == 0) {
				printf("Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? y/n\n");
				char c;
				viderBuffer ();
				scanf ("%c", &c);
				if (c == 'y') {
					printf("Vous avez abandonné le combat, vous avez donc perdu\n");
					gagnant = 3;
					break;
				}
				
			}
			else {
				printf ("Entrez un nombre valide !");
			}
		}
		tour++;
	}

	return gagnant;
}

int attaque (Ipmon* ipmon1, Ipmon* ipmon2) {//Retourne le pv restant de l'ipmon2, car c'est lui qui subit l'attaque, et -1 s'il n'a plus de vie
	int degat;
	int attaque_reussie; //Variable qui va déterminer si une attaque est réussie ou ratée
	attaque_reussie = rand () % 5; //20% de chance qu'une attaque simple soit ratée
	if (attaque_reussie != 0) { //Si l'attaque n'est pas ratée
		degat = ((((ipmon1->niveau + 0.4 + 2) * (ipmon1->puissance_attaque)) / ((ipmon2->puissance_defense) * 50)) + 2) * ((ipmon1->typeEntier) / (ipmon2->typeEntier));
		if (strcmp(ipmon1->type, ipmon2->type)) { //Si les deux ipmons sont de même type (le degat augmente de 50%)
			degat += (degat / 2) ;
		}
	}
	else {
		return ipmon2->pv;
	}

	if (degat < ipmon2->pv) //Si il reste encore des pv à ipmon, on lui retire le degat subit
		ipmon2->pv -= degat;
	else
		return -1;

	return ipmon2->pv;
}

int attaque_speciale (Ipmon* ipmon1, Ipmon* ipmon2) {
	int degat;
	int attaque_reussie; //Variable qui va déterminer si une attaque est réussie ou ratée
	attaque_reussie = rand () % 2; //50% de chance qu'une attaque spéciale soit ratée
	if (attaque_reussie != 0) { //Si l'attaque n'est pas ratée
		degat = ((((ipmon1->niveau + 0.4 + 2) * (ipmon1->puissance_attaque_special)) / ((ipmon2->puissance_defense_special) * 50)) + 2) * ((ipmon1->typeEntier) * (ipmon2->typeEntier));
		if (strcmp(ipmon1->type, ipmon2->type)) { //Si les deux ipmons sont de même type (le degat augmente de 50%)
			degat += (degat / 2) ;
		}
	}
	else {
		return ipmon2->pv;
	}

	if (degat < ipmon2->pv) //Si il reste encore des pv à ipmon, on lui retire le degat subit
		ipmon2->pv -= degat;
	else
		return -1;

	return ipmon2->pv;
}

int Combat2Dresseurs (Dresseur dresseur1, Dresseur dresseur2) { //Retourne 1 si dresseur1 gagne, 2 si dresseur2 gagne, 0 si match nul et 3 si dresseur abandonne
	int gagnant = 0;
	int id, i;
	char *pseudo1, *pseudo2;
	pseudo1 = dresseur1.pseudo;
	pseudo2 = dresseur2.pseudo;
	Ipmon ipmonDresseur1, ipmonDresseur2;

	printf("**************************************\n");
	printf("dresseur1 : \n\tPseudo : %s\n\tNiveau : %d\n", pseudo1, dresseur1.niveau);
	printf("dresseur2 : \n\tPseudo : %s\n\tNiveau : %d\n\n", pseudo2, dresseur2.niveau);

	printf("Liste de tous les ipmons du dresseur : %s\n", pseudo1);
	for (i = 0; i < dresseur1.nombreIpmons; i++) {
		printf("%s, id = %d\n", dresseur1.ipmons[i].nom, dresseur1.ipmons[i].id);
	}

	printf("Liste de tous les ipmons du dresseur : %s\n", pseudo2);
	for (i = 0; i < dresseur2.nombreIpmons; i++) {
		printf("%s, id = %d\n", dresseur2.ipmons[i].nom, dresseur2.ipmons[i].id);
	}

	do {	
		printf("%s Entrez l'id du pokemon choisi pour ce combat\n", pseudo1);
		scanf ("%d", &id);
		ipmonDresseur1 = chercherIpmonDeDresseur (dresseur1, id);
		viderBuffer ();
	}while (ipmonDresseur1.id == -1);

	printf("%s a choisi l'ipmon : %s\n\n", pseudo1, ipmonDresseur1.nom);

	do {	
		printf("%s Entrez l'id du pokemon choisi pour ce combat\n", pseudo2);
		scanf ("%d", &id);
		ipmonDresseur2 = chercherIpmonDeDresseur (dresseur2, id);
		viderBuffer ();
	}while (ipmonDresseur2.id == -1);

	printf("%s a choisi l'ipmon : %s\n\n", pseudo2, ipmonDresseur2.nom);

	srand (time (NULL));

	int tour = 1, choix;

	if (ipmonDresseur1.agilite > ipmonDresseur2.agilite)
		printf("L'ipmon de %s, %s commence le jeu\n\n", pseudo1, ipmonDresseur1.nom);
	else
		printf("L'ipmon de %s, %s commence le jeu\n\n", pseudo2, ipmonDresseur2.nom);

	//Debut du combat
	while ((ipmonDresseur1.pv > 0) && (ipmonDresseur2.pv > 0)) { //Tant que l'un des ipmons ne meurt pas
		printf("Tour %d\n", tour);
		if (ipmonDresseur1.agilite > ipmonDresseur2.agilite) { //L'ipmon du dresseur1 est plus rapide que l'ipmon du dresseur2, donc c'est lui qui attaque en 1er
			do {	
				printf("%s, Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n", pseudo1);
				scanf ("%d", &choix);
				if (choix == 1) {
					int x = ipmonDresseur2.pv;
					ipmonDresseur2.pv = attaque (&ipmonDresseur1, &ipmonDresseur2);
					printf("%s a fait l'attaque : %s\n", pseudo1, ipmonDresseur1.nom_attaque);
					if (x == ipmonDresseur2.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur2.pv == -1) { //Si l'ipmon du dresseur2 n'a plus de vie
							gagnant = 1;
							break;
						}
						printf("Il reste %d pv à l'ipmon de %s\n", ipmonDresseur2.pv, pseudo2);
					}
				}
				else if (choix == 2) {
					int x = ipmonDresseur2.pv;
					ipmonDresseur2.pv = attaque_speciale (&ipmonDresseur1, &ipmonDresseur2);
					printf("%s a fait l'attaque spéciale : %s\n", pseudo1, ipmonDresseur1.nom_attaque_special);
					if (x == ipmonDresseur2.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur2.pv == -1) { //Si l'ipmon du dresseur2 n'a plus de vie
							gagnant = 1;
							break;
						}
						printf("Il reste maintenant %d pv à l'ipmon de %s\n", ipmonDresseur2.pv, pseudo2);
					}
				}
				else if (choix == 0) {
					printf("%s , Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? 1 : oui, 0 : non\n", pseudo1);
					int a;
					viderBuffer ();
					scanf("%d", &a);
					if (a == 1) {
						printf("%s a abandonné le combat, il a donc perdu\n", pseudo1);
						gagnant = 2;
						return gagnant;
					}
				}
				viderBuffer ();
			} while (choix != 1 && choix != 2 && choix != 0);
			if (ipmonDresseur2.pv < 0) { //Si l'ipmon du dresseur2 n'a plus de vie
				gagnant = 1;
				break;
			}
			do {	
				printf("%s, Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n", pseudo2);
				scanf ("%d", &choix);
				if (choix == 1) {
					int x = ipmonDresseur1.pv;
					ipmonDresseur1.pv = attaque (&ipmonDresseur2, &ipmonDresseur1);
					printf("%s a fait l'attaque : %s\n", pseudo2, ipmonDresseur2.nom_attaque);
					if (x == ipmonDresseur1.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur1.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il reste %d pv à l'ipmon de %s\n", ipmonDresseur1.pv, pseudo1);
					}
				}
				else if (choix == 2) {
					int x = ipmonDresseur1.pv;
					ipmonDresseur1.pv = attaque_speciale (&ipmonDresseur2, &ipmonDresseur1);
					printf("%s a fait l'attaque spéciale : %s\n", pseudo2, ipmonDresseur2.nom_attaque_special);
					if (x == ipmonDresseur1.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur1.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il reste maintenant %d pv à l'ipmon de %s\n", ipmonDresseur1.pv, pseudo1);
					}
				}
				else if (choix == 0) {
					printf("%s , Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? 1 : oui, 0 : non\n", pseudo2);
					int a;
					viderBuffer ();
					scanf("%d", &a);
					if (a == 1) {
						printf("%s a abandonné le combat, vous avez donc perdu\n", pseudo2);
						gagnant = 1;
						return gagnant;
					}
				}
				viderBuffer ();
			} while (choix != 1 && choix != 2 && choix != 0);
			if (ipmonDresseur1.pv < 0) { //Si l'ipmon du dresseur2 n'a plus de vie
				gagnant = 2;
				break;
			}
		}
		else {
			do {	
				printf("%s, Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n", pseudo2);
				scanf ("%d", &choix);
				if (choix == 1) {
					int x = ipmonDresseur1.pv;
					ipmonDresseur1.pv = attaque (&ipmonDresseur2, &ipmonDresseur1);
					printf("%s a fait l'attaque : %s\n", pseudo2, ipmonDresseur2.nom_attaque);
					if (x == ipmonDresseur1.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur1.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il reste %d pv à l'ipmon de %s\n", ipmonDresseur1.pv, pseudo1);
					}
				}
				else if (choix == 2) {
					int x = ipmonDresseur1.pv;
					ipmonDresseur1.pv = attaque_speciale (&ipmonDresseur2, &ipmonDresseur1);
					printf("%s a fait l'attaque spéciale : %s\n", pseudo2, ipmonDresseur1.nom_attaque_special);
					if (x == ipmonDresseur1.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur1.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 2;
							break;
						}
						printf("Il reste maintenant %d pv à l'ipmon de %s\n", ipmonDresseur1.pv, pseudo1);
					}
				}
				else if (choix == 0) {
					printf("%s , Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? 1 : oui, 0 : non\n", pseudo2);
					int a;
					viderBuffer ();
					scanf("%d", &a);
					if (a == 1) {
						printf("%s a abandonné le combat, vous avez donc perdu\n", pseudo2);
						gagnant = 1;
						return gagnant;
					}
				}
				viderBuffer ();
			} while (choix != 1 && choix != 2 && choix != 0);
			if (ipmonDresseur1.pv < 0) { //Si l'ipmon du dresseur2 n'a plus de vie
				gagnant = 2;
				break;
			}
			do {	
				printf("%s, Appuyer sur 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner le combat!\n", pseudo1);
				scanf ("%d", &choix);
				if (choix == 1) {
					int x = ipmonDresseur1.pv;
					ipmonDresseur2.pv = attaque (&ipmonDresseur1, &ipmonDresseur2);
					printf("%s a fait l'attaque : %s\n", pseudo1, ipmonDresseur1.nom_attaque);
					if (x == ipmonDresseur2.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur2.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 1;
							break;
						}
						printf("Il reste %d pv à l'ipmon de %s\n", ipmonDresseur2.pv, pseudo2);
					}
				}
				else if (choix == 2) {
					int x = ipmonDresseur2.pv;
					ipmonDresseur2.pv = attaque_speciale (&ipmonDresseur1, &ipmonDresseur2);
					printf("%s a fait l'attaque spéciale : %s\n", pseudo1, ipmonDresseur1.nom_attaque_special);
					if (x == ipmonDresseur2.pv)
						printf("Attaque ratée !\n");
					else {
						if (ipmonDresseur2.pv == -1) { //Si l'ipmon du dresseur1 n'a plus de vie
							gagnant = 1;
							break;
						}
						printf("Il reste maintenant %d pv à l'ipmon de %s\n", ipmonDresseur2.pv, pseudo2);
					}
				}
				else if (choix == 0) {
					printf("%s , Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? 1 : oui, 0 : non\n", pseudo1);
					int a;
					viderBuffer ();
					scanf("%d", &a);
					if (a == 1) {
						printf("%s a abandonné le combat, il a donc perdu\n", pseudo1);
						gagnant = 2;
						return gagnant;
					}
				}
				viderBuffer ();
			} while (choix != 1 && choix != 2 && choix != 0);
			if (ipmonDresseur2.pv < 0) { //Si l'ipmon du dresseur2 n'a plus de vie
				gagnant = 1;
				break;
			}
		}
		tour++;
	}


	return gagnant;
}


int CombatEquipe (Equipe* equipe1, Equipe* equipe2) {
	//int gagnant;

	
	return 0;
}


// int main () {
// 	Dresseur dresseur1, dresseur2;
// 	Ipmon ipmon, ipmonDresseur1, ipmonDresseur2;

// 	dresseur1.pseudo = "Moussier";
// 	dresseur1.niveau = 3;
// 	dresseur1.nombreIpmons = 1;

// 	dresseur2.pseudo = "Dinar";
// 	dresseur2.niveau = 3;
// 	dresseur2.nombreIpmons = 1;

// 	ipmon.id = 1;
// 	ipmon.nom = "Gaga";
// 	ipmon.etat = "sauvage";
// 	ipmon.type = "Feu";
// 	ipmon.typeEntier = 3;
// 	ipmon.pv = 103;
// 	ipmon.agilite = 50;
// 	ipmon.niveau = 30;
// 	ipmon.puissance_attaque = 87;
// 	ipmon.nom_attaque = "Tornade de feu";
// 	ipmon.precision_attaque = 34;
// 	ipmon.puissance_defense = 90;
// 	ipmon.nom_defense = "Defense suprême de feu";
// 	ipmon.esquive = 100;
// 	ipmon.precision = 4;
// 	ipmon.puissance_attaque_special = 95;
// 	ipmon.nom_attaque_special = "Mega Tornade de feu";
// 	ipmon.precision_attaque_special = 6;
// 	ipmon.puissance_defense_special = 99;
// 	ipmon.nom_defense_special = "Mega Defense suprême de feu";

// 	ipmonDresseur1.id = 2;
// 	ipmonDresseur1.nom = "Gogo";
// 	ipmonDresseur1.etat = "capturé";
// 	ipmonDresseur1.type = "Eau";
// 	ipmonDresseur1.typeEntier = 3;
// 	ipmonDresseur1.pv = 320;
// 	ipmonDresseur1.agilite = 30;
// 	ipmonDresseur1.niveau = 30;
// 	ipmonDresseur1.puissance_attaque = 50;
// 	ipmonDresseur1.nom_attaque = "Pluie de bulle";
// 	ipmonDresseur1.precision_attaque = 34;
// 	ipmonDresseur1.puissance_defense = 98;
// 	ipmonDresseur1.nom_defense = "Grand Ouragan";
// 	ipmonDresseur1.esquive = 100;
// 	ipmonDresseur1.precision = 4;
// 	ipmonDresseur1.puissance_attaque_special = 170;
// 	ipmonDresseur1.nom_attaque_special = "Mega Pluie de bulle";
// 	ipmonDresseur1.precision_attaque_special = 56;
// 	ipmonDresseur1.puissance_defense_special = 105;
// 	ipmonDresseur1.nom_defense_special = "Mega Grand Ouragan";

// 	ipmonDresseur2.id = 3;
// 	ipmonDresseur2.nom = "Biju";
// 	ipmonDresseur2.etat = "capturé";
// 	ipmonDresseur2.type = "Plante";
// 	ipmonDresseur2.typeEntier = 3;
// 	ipmonDresseur2.pv = 120;
// 	ipmonDresseur2.agilite = 30;
// 	ipmonDresseur2.niveau = 30;
// 	ipmonDresseur2.puissance_attaque = 50;
// 	ipmonDresseur2.nom_attaque = "Etranglement des 1000 branches";
// 	ipmonDresseur2.precision_attaque = 34;
// 	ipmonDresseur2.puissance_defense = 98;
// 	ipmonDresseur2.nom_defense = "Forêt Vivante";
// 	ipmonDresseur2.esquive = 100;
// 	ipmonDresseur2.precision = 4;
// 	ipmonDresseur2.puissance_attaque_special = 170;
// 	ipmonDresseur2.nom_attaque_special = "Super Etranglement des 1000 branches";
// 	ipmonDresseur2.precision_attaque_special = 56;
// 	ipmonDresseur2.puissance_defense_special = 105;
// 	ipmonDresseur2.nom_defense_special = "Mega Forêt géante";

// 	dresseur1.ipmons[0] = ipmon;
// 	dresseur2.ipmons[0] = ipmonDresseur2;
// 	int choix;
// 	do {	
// 		printf("************************************\n");
// 		printf("1 - Combat Dresseur vs Ipmon sauvage\n");
// 		printf("2 - Combat Dresseur vs Dresseur\n");
// 		printf("3 - Combat en équipe\n");
// 		printf("4 - Echange d'ipmons\n");
// 		printf("5 - Infos dresseurs\n");
// 		scanf ("%d", &choix);
// 		viderBuffer ();
// 	} while (choix < 1 && choix > 5);

// 	int c = -1;
// 	if (choix == 1) 
// 		c = CombatDresseurIpmon (dresseur1, ipmon);
// 	else if (choix == 2)
// 		c = Combat2Dresseurs (dresseur1, dresseur2);
// 	else if (choix == 3)
// 		printf("Ce combat n'est pas encore pris en charge\n");
// 	else if (choix == 4) {
// 		printf("Avant\n");
// 		printf("Dresseur1 : %s\n", dresseur1.pseudo);
// 		int i;
// 		for (i = 0; i < dresseur1.nombreIpmons; i++) {
// 			printf("%s\n", dresseur1.ipmons[i].nom);
// 		}
// 		printf("\n");
// 		printf("Dresseur2 : %s\n", dresseur2.pseudo);
// 		for (i = 0; i < dresseur2.nombreIpmons; i++) {
// 			printf("%s\n", dresseur2.ipmons[i].nom);
// 		}
// 		EchangerIpmons (&dresseur1, &dresseur2, ipmon, ipmonDresseur2);
// 		printf("Après\n");
// 		printf("Dresseur1 : %s\n", dresseur1.pseudo);
// 		for (i = 0; i < dresseur1.nombreIpmons; i++) {
// 			printf("%s\n", dresseur1.ipmons[i].nom);
// 		}
// 		printf("\n");
// 		printf("Dresseur2 : %s\n", dresseur2.pseudo);
// 		for (i = 0; i < dresseur2.nombreIpmons; i++) {
// 			printf("%s\n", dresseur2.ipmons[i].nom);
// 		}
// 	}
// 	else if (choix == 5) 
// 		recupererInfoDresseurs ();

// 	if (choix == 1) {
// 		if (c == 0)
// 			printf("c = %d Match nul\n", c);
// 		else if (c == 1)
// 			printf("Ipmon sauvage %s K.O. !!!    %s a gagné\n", ipmon.nom, dresseur1.pseudo);
// 		else
// 			printf("%s K.O.    !!!!    L'ipmon sauvage %s a gagné\n", dresseur2.pseudo, ipmon.nom);
// 		}
// 	else if (choix == 2) {
// 		if (c == 0)
// 			printf("c = %d Match nul\n", c);
// 		else if (c == 1)
// 			printf("%s K.O. !!!    %s a gagné\n", dresseur2.pseudo, dresseur1.pseudo);
// 		else
// 			printf("%s K.O.    !!!!    %s a gagné\n", dresseur1.pseudo, dresseur2.pseudo);
// 	}

// 	return 0;
// }