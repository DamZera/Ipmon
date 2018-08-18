#include "jeu.h"

int test = 0;
Dresseur_aff *dresseur_list_jeu;
Dresseur* joueur;

void jeuDeplacement(SDL_Rect perso, Coord *persoAvant, int sock){
	char buf[80];
	bzero(buf,80);
	if(perso.x != persoAvant->x || perso.y != persoAvant->y){
		
		sprintf(buf, "010%d:%d", perso.x, perso.y);
		
		send(sock,buf,strlen(buf),0);
		recv(sock,buf,80,0);
		bzero(buf,80);
		persoAvant->y = perso.y;
		persoAvant->x = perso.x;
	}
}

void jeuInitjoueur(Dresseur* joueurInit){
	joueur = joueurInit;
}

Dresseur_aff* jeuAfficherDresseurs(int sock,char* map, TTF_Font *police){
	char buf[80];
	char pseudo [80];
	char* token;
	int coodX,coodY;
	bzero(buf,80);
	int n, end = 0;
	
	SDL_Surface* dresseurB;
	SDL_Surface* texte = NULL;
	SDL_Rect perso, positiontexte;
	perso.w = 25;
	perso.h = 25;
	positiontexte = perso;
	
	sprintf(buf,"011%s",map);
	send(sock,buf,strlen(buf),0);
	bzero(buf,80);
	
	dresseur_list_jeu = vider_dresseur_liste(dresseur_list_jeu);
	
	while(end == 0 && (n = recv(sock,buf,80,0))){
		test++;
		printf("buf : %s Test %d\n",buf,test);
		if(strcmp(buf,"000end")==0)
			end = 1;
		else{
			printf("jeuAfficherDresseurs :: %s\n",buf);
			token = strtok(buf, ":");
    		strcpy(pseudo,token);
   			token = strtok(NULL, ":");
   			coodX = strtol(token, NULL, 10);
   			token = strtok(NULL, ":");
   			coodY = strtol(token, NULL, 10);
   			token = strtok(NULL, ":");
   			
   			SDL_Color couleurNoire = {0, 0, 0}; //couleur noir
   			texte = TTF_RenderText_Blended(police, pseudo, couleurNoire);
   			perso.x = coodX;
   			perso.y = coodY;
   			
   			positiontexte.x = perso.x - (texte->w /2) + (perso.w/2);
			positiontexte.y = perso.y -10;
			dresseur_list_jeu = ajouter_dresseur_aff(dresseur_list_jeu,&perso,&positiontexte,texte);
			send(sock,"recu",strlen("recu"),0);
		}
		
		bzero(buf,80);
		bzero(pseudo,80);
	}
	return dresseur_list_jeu;
}

Dresseur_aff* ajouter_dresseur_aff(Dresseur_aff *dresseur_list_jeu, SDL_Rect *perso, SDL_Rect *positionTexte, SDL_Surface *texte){

    Dresseur_aff* dresseur = malloc(sizeof(Dresseur_aff));
 
    dresseur->texte = texte;
    dresseur->perso = *perso;
    dresseur->positiontexte = *positionTexte;
    dresseur->next = NULL;
 
    if(dresseur_list_jeu == NULL){
        return dresseur;
    }
    else
    {
        Dresseur_aff* tmp = dresseur_list_jeu;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = dresseur;
        return dresseur_list_jeu;
    }
}

Dresseur_aff* vider_dresseur_liste(Dresseur_aff *dresseur_list_jeu){
	Dresseur_aff* dresseur = dresseur_list_jeu;
	Dresseur_aff* tmp;
	
	
	if(dresseur != NULL){
		while(dresseur->next != NULL){
			tmp = dresseur;
            dresseur = dresseur->next;
            free(tmp);
    	}
    	free(dresseur);
    }
    return NULL;
}
int jeuCombat(int sock){
	int combat = 0, idpokemonAdv = 0, end = 0, msg;
	char buf[80]; bzero(buf,80);
	//char* token;
	
	combat = rand()%300;
	
	if(combat<3){
		idpokemonAdv = rand()%50+1;
		printf("Combat contre pokemon id = %d\n",idpokemonAdv);
		/*send(sock,buf,strlen(buf),0);
		bzero(buf,80);
		printf("Socket = %d\n",sock );
		bzero(buf,80);
		while((end == 0) && (recv(sock,buf,80,0))){
			printf("buff : %s",buf);
			Ipmon *ipmon_adv = malloc(sizeof(Ipmon));
			token = strtok(buf, ":");
			printf("Buf : %s\n",buf);
			if(strcmp(token,"msg") == 0){
				token = strtok(NULL, ":");
				msg = strtol(token,NULL, 10);
				send(sock,"recu",strlen("recu"),0);
			} else if(strcmp(buf,"end") == 0){
				end = 1;
			}
			else{
				printf("%s",buf);
				send(sock,"recu",strlen("recu"),0);
			}
			affichage_combat(msg, sock);
			bzero(buf,80);
		}*/
	}
}

int affichage_combat(int msg, int sock){
	
	printf("STUBED must be reworked");
	//char buf[80];
	//char* token;
	//int choix_ipmon;
	//Ipmon *ipmon = joueur->ipmons;
	//Ipmon *ipmon_adv;
	//joueur->niveau = 1;

	// switch(msg){
	// 	case 1:	printf("**************************************\n");
	// 			printf("Dresseur : \n\tPseudo : %s\n\tNiveau : %d\n", joueur->pseudo, joueur->niveau);

	// 			printf("Liste de tous les ipmons du dresseur\n");
	// 			while(ipmon != NULL){
	// 				printf("\n\nIpmon : \n\tId : %d\n\tNom : %s\n\tEtat : %s\n\tType : %s\n\tPoint de vie : %d\n\tAgilité : %d\n\tNiveau : %d\n\tPuissance attaque : %d\n\tNom attaque : %s\n\tPuissance défense : %d\n\t", ipmon->id, ipmon->nom, ipmon->etat, ipmon->type, ipmon->pv, ipmon->agilite, ipmon->niveau, ipmon->puissance_attaque, ipmon->nom_attaque, ipmon->puissance_defense);
	// 				ipmon = ipmon->next;
	// 			}
	// 			printf("Entrer l'id du pokemon choisi :\n");
	// 		scanf("%d",&choix_ipmon);
	// 		sprintf(buf,"%d",choix_ipmon);
	// 		send(sock,buf,strlen(buf),0);
	// 		break;
	// 	case 2:
	// 		recv(sock,buf,80,0);
	// 		token = strtok(buf, ":");
	// 		ipmon_adv->id = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->nom = token;
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->etat = token;
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->type = token;
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->typeEntier = strtol(token,NULL,10);
	// 		token = NULL;


	// 		bzero(buf,80);
	// 		send(sock,"recu",strlen("recu"),0);
	// 		recv(sock,buf,80,0);
	// 		token = strtok(buf, ":");
	// 		ipmon_adv->pv = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->agilite = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->niveau = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->puissance_attaque = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->nom_attaque = token;
	// 		token = NULL;

	// 		bzero(buf,80);
	// 		send(sock,"recu",strlen("recu"),0);
	// 		recv(sock,buf,80,0);
	// 		token = strtok(buf, ":");
	// 		ipmon_adv->precision_attaque = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->puissance_defense = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->nom_defense = token;
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->esquive = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->precision = strtol(token,NULL,10);
	// 		token = NULL;

	// 		bzero(buf,80);
	// 		send(sock,"recu",strlen("recu"),0);
	// 		recv(sock,buf,80,0);
	// 		token = strtok(buf, ":");
	// 		ipmon_adv->puissance_attaque_special = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->precision_attaque_special = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->nom_attaque_special = token;
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->puissance_defense_special = strtol(token,NULL,10);
	// 		token = strtok(NULL, ":");
	// 		ipmon_adv->nom_defense_special = token;
	// 		token = NULL;

	// 		bzero(buf,80);
	// 		printf("\nIpmon sauvage : \n\tId : %d\n\tNom : %s\n\tEtat : %s\n\tType : %s\n\tPoint de vie : %d\n\tAgilité : %d\n\tNiveau : %d\n\tPuissance attaque : %d\n\tNom attaque : %s\n\tPuissance défense : %d\n\tNom défense : %s\n\t", ipmon_adv->id, ipmon_adv->nom, ipmon_adv->etat, ipmon_adv->type, ipmon_adv->pv, ipmon_adv->agilite, ipmon_adv->niveau, ipmon_adv->puissance_attaque, ipmon_adv->nom_attaque, ipmon_adv->puissance_defense, ipmon_adv->nom_defense);
	// 		printf("Entrer l'id du pokemon choisi :\n");
	// 		scanf("%d",&choix_ipmon);
	// 		sprintf(buf,"%d",choix_ipmon);
	// 		send(sock,buf,strlen(buf),0);
	// 		break;
	// 	case 3:
	// 		printf("Appuyer: 1 pour faire une attaque simple, 2 pour une attaque spéciale et 0 pour abandonner!\n");
	// 		scanf("%d", &choix_ipmon);
	// 		sprintf(buf,"%d",choix_ipmon);
	// 		send(sock,buf,strlen(buf),0);
	// 		break;
	// 	case 4:
	// 		printf("Si vous abandonnez, vous allez perdre le combat; Voulez-vous quitter ? y/n\n");
	// 		char c;
	// 		scanf("%c", &c);
	// 		sprintf(buf,"%c", c);
	// 		send(sock,buf,strlen(buf),0);
	// 	default:

	// 		break;
	// }
}