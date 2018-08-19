#include "log.h"
#include "mysql_ipmon.h"
#include "jeu_serv.h"

#define DEBUG 1
#define TAILLE_BUFF 200

Dresseur *dresseur_list = NULL;
/*Mutex pour les zones critiques*/
pthread_mutex_t mutex_client = PTHREAD_MUTEX_INITIALIZER;

Dresseur* chercher_dresseur(Dresseur *dresseur_list,int socket){
	Dresseur* dresseur = dresseur_list;
	while(dresseur != NULL){
		if(dresseur->sock == socket)
			return dresseur;
		else
			dresseur = dresseur->next;
	}

	return NULL;
}

Dresseur* ajouter_dresseur(Dresseur *dresseur_list, int socket, char* pseudo, int coodX, int coodY, char* map, int id){

    Dresseur* dresseur = malloc(sizeof(Dresseur));
 
    dresseur->id = id;
    dresseur->sock = socket;
    dresseur->pseudo = (char*)malloc(sizeof(char)*strlen(pseudo)+1);
    dresseur->map = (char*)malloc(sizeof(char)*strlen(map)+1);
    strcpy(dresseur->pseudo, pseudo);
    strcpy(dresseur->map, map);
    dresseur->next = NULL;
    dresseur->coodX = coodX;
    dresseur->coodY = coodY;
 
    if(dresseur_list == NULL){
        return dresseur;
    }
    else
    {
        Dresseur* tmp = dresseur_list;
        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = dresseur;
        return dresseur_list;
    }
}

Ipmon* cree_Ipmon(int id, char* nom, char* etat, char* type, int typeEntier 	, int pv 	, int agilite , int niveau 	, int puissance_attaque , char* nom_attaque, int precision_attaque, int puissance_defense , char* nom_defense, int esquive 	, int precision 	, int puissance_attaque_special, int precision_attaque_special, char* nom_attaque_special, int puissance_defense_special	, char* nom_defense_special){
	Ipmon* ipmon = malloc(sizeof(Ipmon));

	ipmon->id = id;
	snprintf(ipmon->nom, MAX_SIZE_IPMON_STR, "%s", nom);
	snprintf(ipmon->etat, MAX_SIZE_IPMON_STR, "%s", etat);
	snprintf(ipmon->type, MAX_SIZE_IPMON_STR, "%s", type);
	ipmon->typeEntier = typeEntier; 
	ipmon->pv = pv; 
	ipmon->agilite = agilite; 
	ipmon->niveau = niveau; 
	ipmon->puissance_attaque = puissance_attaque; 
	snprintf(ipmon->nom_attaque, MAX_SIZE_IPMON_STR, "%s", nom_attaque);
	ipmon->precision_attaque = precision_attaque;
	ipmon->puissance_defense = puissance_defense; 
	snprintf(ipmon->nom_defense, MAX_SIZE_IPMON_STR, "%s", nom_defense);
	ipmon->esquive = esquive; 
	ipmon->precision = precision;
	ipmon->puissance_attaque_special = puissance_attaque_special;
	ipmon->precision_attaque_special = precision_attaque_special;
	snprintf(ipmon->nom_attaque_special, MAX_SIZE_IPMON_STR, "%s", nom_attaque_special);
	ipmon->puissance_defense_special = puissance_defense_special;
	snprintf(ipmon->nom_defense_special, MAX_SIZE_IPMON_STR, "%s", nom_defense_special);

	return ipmon;
}

void afficher_dresseur(Dresseur *dresseur_list){
	Dresseur* dresseur = dresseur_list;
	int i = 0;
	printf("Liste des dresseurs connecté :\n");
	while(dresseur != NULL){
		printf(" Dresseur %d : %s map : %s cood %d:%d sock %d\n",i,dresseur->pseudo,dresseur->map,dresseur->coodX,dresseur->coodY,dresseur->sock);
		dresseur = dresseur->next; i++;
	}
}


Dresseur* supprimer_dresseur(Dresseur *dresseur_list, int socket){
	int i;
	
	if(dresseur_list == NULL)
		return NULL;
		
	if(dresseur_list->sock == socket){
		Dresseur* dresseur = dresseur_list->next;
		free(dresseur_list);
		dresseur = supprimer_dresseur(dresseur, socket);
			
		return dresseur;
	}else{
		dresseur_list->next = supprimer_dresseur(dresseur_list->next, socket);
		return dresseur_list;
	}
}

void envoieIpmonDresseur(int s_dial,MYSQL* ipmon_bdd, Dresseur* dresseur_list){
	Dresseur* dresseur = chercher_dresseur(dresseur_list, s_dial);
	char requete[150];
	int i;
	char buf[MAX_SIZE_IPMON_STR];
	bzero(buf,MAX_SIZE_IPMON_STR);
	Ipmon* ipmon_adv = malloc(sizeof(Ipmon));
	MYSQL_RES *result;
	MYSQL_ROW row;

	sprintf(requete,"SELECT * FROM ipdex,ipmon WHERE ipdex.dresseur = '%d' AND ipdex.ipmon = ipmon.Id_IPmon", dresseur->id);
	printf("%s\n",requete);

	if(mysql_query(ipmon_bdd, requete)){
    			finish_with_error(ipmon_bdd);
	} 
	result = mysql_store_result(ipmon_bdd);
	row = mysql_fetch_row(result);
	while(row){
		for(i=0;i<20;i++)
			printf("%s ",row[i]);
		printf("\n"); 
		ipmon_adv->id = strtol(row[3],NULL,10);ipmon_adv->typeEntier = strtol(row[7],NULL,10);
		
		snprintf(ipmon_adv->nom, MAX_SIZE_IPMON_STR, "%s", row[4]);
		snprintf(ipmon_adv->etat, MAX_SIZE_IPMON_STR, "%s", row[5]);
		snprintf(ipmon_adv->type, MAX_SIZE_IPMON_STR, "%s", row[6]);
		snprintf(ipmon_adv->nom_attaque, MAX_SIZE_IPMON_STR, "%s", row[12]);
		snprintf(ipmon_adv->nom_attaque_special, MAX_SIZE_IPMON_STR, "%s", row[18]);
		
		
		ipmon_adv->pv = strtol(row[8],NULL,10);ipmon_adv->agilite = strtol(row[9],NULL,10);ipmon_adv->niveau = strtol(row[1],NULL,10);ipmon_adv->puissance_attaque = strtol(row[10],NULL,10);
	
		ipmon_adv->precision_attaque = strtol(row[11],NULL,10);ipmon_adv->puissance_defense = strtol(row[13],NULL,10);ipmon_adv->esquive = strtol(row[14],NULL,10);ipmon_adv->precision = strtol(row[15],NULL,10);
	
		ipmon_adv->puissance_attaque_special = strtol(row[16],NULL,10);ipmon_adv->precision_attaque_special = strtol(row[17],NULL,10);ipmon_adv->puissance_defense_special = strtol(row[19],NULL,10);

		// MY EYES please need rework

		// printf("%d:%s:%s:%s:%d\n",ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier);
		// sprintf(buf,"%d:%s:%s:%s:%d",ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier);
		// send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// sprintf(buf,"%d:%d:%d:%d:%s",ipmon_adv->pv,ipmon_adv->agilite,ipmon_adv->niveau,ipmon_adv->puissance_attaque,ipmon_adv->nom_attaque);
		// send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// sprintf(buf,"%d:%d:%d:%d",ipmon_adv->precision_attaque,ipmon_adv->puissance_attaque,ipmon_adv->esquive,ipmon_adv->precision);
		// send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// sprintf(buf,"%d:%d:%s:%d",ipmon_adv->puissance_attaque_special,ipmon_adv->precision_attaque_special,ipmon_adv->nom_attaque_special,ipmon_adv->puissance_defense_special);
		// send(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		// bzero(buf,MAX_SIZE_IPMON_STR);
		// recv(s_dial,buf,MAX_SIZE_IPMON_STR,0);
		row = mysql_fetch_row(result);
	}
	send(s_dial,"end",strlen("end"),0);
}


/*Fonction qui lance les fonctions SQL si on recoit un message attendu*/
int query_ipmonbdd(Message *msg, int *s_dial){
	int choice = 5;
	if(strcmp(msg->code,"001") == 0){
		 if(strcmp(msg->data, "insert_dresseur") == 0){
			choice = 0;
		 }
		 else if(strcmp(msg->data, "connect") == 0){
		 	choice = 1;
		 }
		 else{
			 send(*s_dial, "000NOPE_UNKNOW", strlen("000NOPE_UNKNOW"),0);
			 choice = 1;
		 }
	}else if(strcmp(msg->code,"999") == 0){
		pthread_mutex_lock(&mutex_client);
		choice = 10;
		dresseur_list =  supprimer_dresseur(dresseur_list, *s_dial);
		afficher_dresseur(dresseur_list);
		pthread_mutex_unlock(&mutex_client);
		printf("\nCLOSE client :: %d\n", *s_dial);
	}
	return choice;
}

int connection_dresseur(int *s_dial, MYSQL* ipmon_bdd, char* pseudo, char* pass){
	char requete[1024] = "";
	char buf [TAILLE_BUFF];
	int connecte = 0, coodX, coodY;
	MYSQL_RES *result;
	MYSQL_ROW row; 

	printf("Client : %d Pseudo :: %s et Mot de Passe : %s\n", *s_dial,pseudo, pass);

	sprintf(requete, "SELECT * FROM dresseur WHERE Nom_Dresseur='%s' AND Mdp_Dresseur='%s';", pseudo, pass);
	printf ("Client : %d Requete :: %s \n",*s_dial, requete);
	/*On execute la requete DEBUT ZONE CRITIQUE*/
	pthread_mutex_lock(&mutex_client);
	 
	if(mysql_query(ipmon_bdd, requete)){
		finish_with_error(ipmon_bdd);
	}   

	result = mysql_store_result(ipmon_bdd);
	row = mysql_fetch_row(result);
	if(row)
	{
		sprintf(buf,"%d:%s:%s:%s", ACCEPT, row[5], row[6], row[9]);
		//row[0]id row[5]Cordonnee X row[6]Coordonnee Y row[9] map
		coodX = strtol(row[5],NULL,10);
		coodY = strtol(row[6],NULL,10);
		dresseur_list = ajouter_dresseur(dresseur_list, *s_dial, pseudo,coodX,coodY,row[9],strtol(row[0],NULL,10));
		//recv(*s_dial, buf, TAILLE_BUFF,0);//recu attente
		//bzero(buf,MAX_SIZE_IPMON_STR);
		/*envoieIpmonDresseur(*s_dial,ipmon_bdd,dresseur_list);*/
		
		
		afficher_dresseur(dresseur_list);
		connecte = 1;
	}
	else
	{
		sprintf(buf,"%d", REFUSE);
	}
	send(*s_dial, buf, strlen(buf),0);
	pthread_mutex_unlock (&mutex_client);

	return connecte;
}

/*CODE PSEUDO = 005 PASS = 006*/
/*Fonction qui communique avec le client pour ajouter un dresseur dans la base : ipmon*/
void insert_dresseur(int *s_dial, MYSQL* ipmon_bdd){
#if (DEBUG>0)
	printf("Insert_Dresseur");
#endif
	int n, end = 0;
	Message *msg = (Message*)malloc(sizeof(Message));
	char* pseudo = NULL;
	char* pass = NULL;
	char buf [TAILLE_BUFF];
	char requete[150] = "";

	send(*s_dial, "001PSEUDO", strlen("001PSEUDO"),0);

	bzero (buf, TAILLE_BUFF) ;

	while (end == 0 && (n = recv(*s_dial, buf, TAILLE_BUFF,0))) {
		snprintf(msg->code, 4, "%s", buf);
		snprintf(msg->data, 100, "%s", buf+3);
#if (DEBUG >0)
		printf ("J'ai recu [%s] \n msg->code : %s\nmsg->data : %s \n", buf, msg->code , msg->data) ;
#endif
		if(strcmp(msg->code,"005") == 0){
			pseudo = (char*)malloc(TAILLE_BUFF*sizeof(char));
			strcpy(pseudo, msg->data);
			send(*s_dial, "001PASS", strlen("001PASS"),0);
			bzero(buf, TAILLE_BUFF);
		}
		if(strcmp(msg->code,"006") == 0){
			pass = (char*)malloc(TAILLE_BUFF*sizeof(char));
			strcpy(pass, msg->data);
			send(*s_dial, "001END", strlen("001END"),0);
			printf("Client : %d Pseudo :: %s et Mot de Passe : %s\n", *s_dial,pseudo, pass);

			/*On ajoute le pseudo et le pass dans la requete*/
			sprintf(requete, "INSERT INTO dresseur VALUES('0','NULL','%s','%s', '1','300','150','0','0','tilesetIPMON.txt');", pseudo, pass);
			printf ("Client : %d Requete :: %s \n", *s_dial,requete);
			/*On execute la requete DEBUT ZONE CRITIQUE*/
			 pthread_mutex_lock(&mutex_client);
			/*On execute la requete*/
			mysql_query(ipmon_bdd, requete);
			/* FIN zone critique */
			pthread_mutex_unlock (&mutex_client);
#if (DEBUG >0)
			printf ("Fin de zone critque query OK !");
#endif
			end = 1;
		}
		bzero(msg->code,50);
		bzero(msg->data,50);
	}
}



/* On cree une socket d'ecoute sur le port donne avec un nombre maximum de
 * nb_max_clients de clients en file d'attente.
 */
int cree_serveur_tcp (int port, int nb_max_clients) {
  int s_ecoute ;
  struct sockaddr_in serv_addr ;

  /* on designe l'adresse+port qu'on va ouvrir */
  serv_addr.sin_family = AF_INET ;
  serv_addr.sin_addr.s_addr = htonl (INADDR_ANY) ; /* on attend sur toutes nos adresses */
  serv_addr.sin_port = htons (port) ;
  memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

  /* on cree la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
  s_ecoute = socket (PF_INET, SOCK_STREAM, 0) ;
  bind (s_ecoute, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;
  listen (s_ecoute, nb_max_clients) ; /* on definit la socket s_ecoute, comme etant une socket d'ecoute*/

  return s_ecoute ;
}

/*
 * Le thread qui gere le client
 */
void *manage_player (void *data) {
	int s_dial = *(int *) data ;
	char buf [TAILLE_BUFF];
	int n, code = 999, connecte = 0;
	char *token, *string, *pstring;
	char pseudo[100];
	char pass[100];

	MYSQL *ipmon_bdd;

	ipmon_bdd = connect_bdd();
	bzero (buf, TAILLE_BUFF) ;


	while (code != CLOSE && (n = recv(s_dial, buf, TAILLE_BUFF,0)))
	{
		pstring = string = strdup(buf);

		if ((token = strsep(&string, ":")) != NULL)
    	{
        	code = strtol(token, NULL, 10);
        	if (code == CONNECTION || code == REGISTER)
        	{
        		token = strsep(&string, ":");
        		if (token != NULL)
        		{
        			strncpy(pseudo, token, 200);
        			token = strsep(&string, ":");
        			if (token != NULL)
        			{
        				strncpy(pass, token, 200);
        			}
        		}
        		
        		if (code == CONNECTION)
        		{
        			if (connection_dresseur(&s_dial, ipmon_bdd, pseudo, pass))
        			{
        				jeu(&s_dial, ipmon_bdd, dresseur_list, &mutex_client);
        			}
        		}
        		else
        		{
        			insert_dresseur(&s_dial, ipmon_bdd);
        		}
        	}
        	else if (code == CLOSE)
        	{
        		printf("CLOSE !");
        	}
    	}

	}

	send (s_dial, "OKCLOSE", strlen("OKCLOSE"),0) ;	
	close (s_dial) ;
	free (data) ;
	return (NULL) ;
}


int main (int argc, char* argv[]) {
	int s_ecoute ;
	int r ;

	// Create socket with a max of MAX_CLIENTS
	s_ecoute = cree_serveur_tcp (strtol(argv[1],NULL,10), MAX_CLIENTS) ;

	while (1) {
		pthread_attr_t *thread_attributes ;
		pthread_t tid ;
		int *s_dial ;

		s_dial = malloc (sizeof *s_dial) ;

		// Connect to socket (return struct to dial with client)
		*s_dial = accept (s_ecoute, NULL, 0) ;

		thread_attributes = malloc (sizeof *thread_attributes) ;
		r = pthread_attr_init (thread_attributes) ;
		r = pthread_attr_setdetachstate (thread_attributes, PTHREAD_CREATE_DETACHED) ;

		r = pthread_create (&tid, thread_attributes, manage_player, s_dial) ;

		r = pthread_attr_destroy (thread_attributes) ;
	}
	close (s_ecoute) ;
	exit (EXIT_SUCCESS) ;
}
