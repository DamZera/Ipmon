#include "main.h"
#include <SDL/SDL.h>

#define LARGEUR_FENETRE 640
#define HAUTEUR_FENETRE 480
#define DEBUG 1

int socket_cli = 0;
Dresseur *dresseur;

Ipmon* ajouterIpmon(Ipmon* ipmon_list,int id,char* nom,char* etat,char* type,int typeEntier,int pv,int agilite,int niveau,int puissance_attaque,char* nom_attaque,int precision_attaque,int puissance_defense,int esquive,int precision,int puissance_attaque_special,int precision_attaque_special,char* nom_attaque_special,int puissance_defense_special){
	Ipmon* ipmon = malloc(sizeof(Ipmon));
	
	ipmon->id=id;
	ipmon->nom = malloc(sizeof(char)*strlen(nom));
	ipmon->etat= malloc(sizeof(char)*strlen(etat));
	ipmon->type= malloc(sizeof(char)*strlen(type));
	ipmon->nom_attaque= malloc(sizeof(char)*strlen(nom_attaque));
	ipmon->nom_attaque_special= malloc(sizeof(char)*strlen(nom_attaque_special));
	ipmon->nom= nom;
	ipmon->etat= etat;
	ipmon->type= type;
	ipmon->typeEntier= typeEntier; //Valeur du type d'ipmon : intervient dans le calcul du succès ou non d'une attaque
	ipmon->pv= pv; //Point de vie
	ipmon->agilite = agilite; //Rapidité de reaction d'un ipmon
	ipmon->niveau= niveau; //Niveau de l'ipmon : intervient dans le calcul du degat subit par un ipmon [1 - 100]
	ipmon->puissance_attaque= puissance_attaque; //Puissance d'attaque simple
	ipmon->nom_attaque= nom_attaque;
	ipmon->precision_attaque= precision_attaque;//Précision de l'attaque simple
	ipmon->puissance_defense= puissance_defense; //Puissance de defense : intervient dans le calcul du degat subit par un ipmon
	ipmon->esquive= esquive; //Capacité d'esquive d'un ipmon : intervient dans le calcul du succès ou non d'une attaque = 100
	ipmon->precision =precision; //Précision de l'ipmon
	ipmon->puissance_attaque_special= puissance_attaque_special;//Puissance de l'attaque spéciale d'un ipmon
	ipmon->precision_attaque_special= precision_attaque_special;//Précision de l'attaque spéciale d'un ipmon
	ipmon->nom_attaque_special= nom_attaque_special;
	ipmon->puissance_defense_special= puissance_defense_special;//Puissance de la défense spéciale d'un ipmon : intervient lorsqu'un ipmon est attaqué par une attaque spéciale

	
	 if(ipmon_list == NULL){
        return ipmon;
    }
    else
    {
        Ipmon* tmp = ipmon_list;
        while(tmp != NULL)
        {
            tmp = tmp->next;
        }
        tmp = ipmon;
        return ipmon_list;
    }
}

void ajoutIpmon(int socket_cli){
		int n, end = 0, i = 0;
		Ipmon* ipmon_adv;
		char* token;
		char buf[80];
		dresseur->ipmons = NULL;
		
		while((end == 0) && recv(socket_cli,buf,80,0)){
			if(strcmp(buf,"end") ==0){
				end = 1;
			}else{
				ipmon_adv =  malloc(sizeof(Ipmon));
				int id;
				char* nom;
				
				ipmon_adv->etat = malloc(sizeof(char)*50);
				ipmon_adv->type = malloc(sizeof(char)*50);
				ipmon_adv->nom_attaque = malloc(sizeof(char)*50);
				ipmon_adv->nom_attaque_special = malloc(sizeof(char)*50);
				printf("buf :: %s pok  %d",buf,i);
				i++;
				token = strtok(buf, ":");
				ipmon_adv->id = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->nom = malloc(sizeof(char)*strlen(token));
				ipmon_adv->nom = token;
				token = strtok(NULL, ":");
				ipmon_adv->etat = malloc(sizeof(char)*strlen(token));
				ipmon_adv->etat = token;
				token = strtok(NULL, ":");
				ipmon_adv->type = malloc(sizeof(char)*strlen(token));
				ipmon_adv->type = token;
				token = strtok(NULL, ":");
				ipmon_adv->typeEntier = strtol(token,NULL,10);;
				token = NULL;


				bzero(buf,80);
				send(socket_cli,"recu",strlen("recu"),0);
				recv(socket_cli,buf,80,0);
				token = strtok(buf, ":");
				ipmon_adv->pv = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->agilite = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->niveau = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->puissance_attaque = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->nom_attaque = malloc(sizeof(char)*strlen(token));
				ipmon_adv->nom_attaque = token;
				token = NULL;

				bzero(buf,80);
				send(socket_cli,"recu",strlen("recu"),0);
				recv(socket_cli,buf,80,0);
				token = strtok(buf, ":");
				ipmon_adv->precision_attaque = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->puissance_defense = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->esquive = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->precision = strtol(token,NULL,10);
				token = NULL;

				bzero(buf,80);
				send(socket_cli,"recu",strlen("recu"),0);
				recv(socket_cli,buf,80,0);
				token = strtok(buf, ":");
				ipmon_adv->puissance_attaque_special = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->precision_attaque_special = strtol(token,NULL,10);
				token = strtok(NULL, ":");
				ipmon_adv->nom_attaque_special = malloc(sizeof(char)*strlen(token));
				ipmon_adv->nom_attaque_special = token;
				token = strtok(NULL, ":");
				printf("buf defence :: %s pok  %d",token,i);
				ipmon_adv->puissance_defense_special = strtol(token,NULL,10);
				token = NULL;
				bzero(buf,80);
				send(socket_cli,"recu",strlen("recu"),0);
				dresseur->ipmons = ajouterIpmon(dresseur->ipmons,ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier,ipmon_adv->pv,ipmon_adv->agilite,ipmon_adv->niveau,ipmon_adv->puissance_attaque,ipmon_adv->nom_attaque,ipmon_adv->precision_attaque,ipmon_adv->puissance_defense,ipmon_adv->esquive,ipmon_adv->precision,ipmon_adv->puissance_attaque_special,ipmon_adv->precision_attaque_special,ipmon_adv->nom_attaque_special,ipmon_adv->puissance_defense_special);
			}
		}
}

gboolean cb_quit (GtkWidget *p_widget, gpointer user_data)
{
    gtk_main_quit();
    return FALSE;
}

int connect_serv_Ipmon(char* adresse, int port){
	int s_cli ;
	int erreur;
	struct sockaddr_in serv_addr ;

	s_cli = socket (PF_INET, SOCK_STREAM, 0) ;
	serv_addr.sin_family = AF_INET ;
	serv_addr.sin_addr.s_addr = inet_addr (adresse) ;
	serv_addr.sin_port = htons (port) ;
	memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));
	erreur = connect (s_cli, (struct sockaddr *)&serv_addr, sizeof serv_addr) ;
	if(erreur == -1)
		s_cli = -1;
	return s_cli;
}

void bouton_enregistrer_clicked(GtkWidget *widget, gpointer data){
	Login *login = data;
	char pseudo[50];
	char pass[50];	
	char buf [80];
	int n , endWhile = 0;
	Message* msg = (Message*)malloc(sizeof(Message));
	bzero(buf, 80);

	strcpy(pseudo,gtk_entry_get_text(GTK_ENTRY(login->champ_login)));
	strcpy(pass,gtk_entry_get_text(GTK_ENTRY(login->champ_pass)));

	send(socket_cli, "001insert_dresseur", strlen("001insert_dresseur"),0);


	while ((endWhile == 0) && (n = recv(socket_cli, buf, 80,0))) {
			bzero(msg->code,4);
			bzero(msg->data,77);
			snprintf(msg->code, 4, "%s", buf);
			snprintf(msg->data, 77, "%s", buf+3);
#if (DEBUG >0)
			printf ("J'ai recu [%s] \n msg->code : %s\nmsg->data : %s \n", buf, msg->code , msg->data) ;
#endif

			if((strcmp(msg->code,"001") == 0) && (strcmp(msg->data,"PSEUDO") == 0)){
				strcpy(buf, "005");
				strcat(buf, pseudo);
				send(socket_cli, buf, strlen(buf)+1,0);
#if (DEBUG >0)
				printf("buff in insert PSEUDO = %s\n", buf);
#endif
				bzero(buf, 80);
			}else if((strcmp(msg->code,"001") == 0) && (strcmp(msg->data,"PASS") == 0)){
				bzero(buf, 80);
				strcpy(buf, "006");
				strcat(buf, pass);
				send(socket_cli, buf, strlen(buf)+1,0);
#if (DEBUG >0)
				printf("buff in insert PASS = %s\n", buf);
#endif
				bzero(buf, 80);
			}else if((strcmp(msg->code,"001") == 0) && (strcmp(msg->data,"END") == 0)){
				gtk_entry_set_text(GTK_ENTRY(login->champ_login),"Enregistrement OK!");
				endWhile = 1;
				bzero(buf, 80);
			}else{
#if (DEBUG >0)
				printf("Message incorecte ou pas de message.\n");
#endif		
				bzero(buf, 80);	
			}
	}
}

gboolean bouton_connect_clicked(GtkWidget *widget, gpointer data){
	Login *login = (Login *)data;
	char buffer [200];
	char *token, *string, *pstring;
	char pseudo[50];
	char pass[50];	
		
	int n ;
	
    // TODO strncpy more secure
	strcpy(pseudo,gtk_entry_get_text(GTK_ENTRY(login->champ_login)));
	strcpy(pass,gtk_entry_get_text(GTK_ENTRY(login->champ_pass)));
	
    bzero(buffer, 200);
    snprintf(buffer, 200, "%d:%s:%s", CONNECTION, pseudo, pass);
    send(socket_cli, buffer, strlen(buffer), 0);

    bzero(buffer, 200);
    n = recv(socket_cli, buffer, 200,0);
    printf("message: %s \n", buffer);
    
    pstring = string = strdup(buffer);
    
    // Parse login message
    if ((token = strsep(&string, ":")) != NULL)
    {
        int code = strtol(token, NULL, 10);
        // Connection OK
        if (code == ACCEPT)
        {
            token = strsep(&string, ":");
            dresseur->coodX = (token != NULL) ? strtol(token, NULL, 10) : 300;
            token = strsep(&string, ":");
            dresseur->coodY = (token != NULL) ? strtol(token, NULL, 10) : 150;
            token = strsep(&string, ":");
            if (token != NULL)
            {
                snprintf(dresseur->map, strlen(token)+1, "%s", token);
            }
            
            sprintf(dresseur->pseudo, "%s", pseudo);
            login->connect = 1;

            printf("LOGIN ok \n");
        }
        // Connection KO
        else if (code == REFUSE)
        {
            gtk_entry_set_text(GTK_ENTRY(login->champ_login),"Login error");
            printf("LOGIN error \n");
        }
    }

    free(pstring);

	if(login->connect == 1){
		//gtk_widget_destroy(login->fenetre);
        printf("CB destroy\n");
        gtk_main_quit ();
        printf("Return False\n");
        return FALSE;
	}
    printf("Return true\n");
	return TRUE;
}

int menu_gtk(){
	Login *login = (Login*)malloc(sizeof(Login));
	GtkWidget* fenetre;  
	GtkWidget *bouton_connect = NULL, *bouton_enregistrer = NULL, *bouton_quitter = NULL;
	GtkWidget *champ_login   = NULL, *champ_pass   = NULL;
	GtkWidget *layout = NULL;
	GtkWidget *background;
	int x_bouton;
        
	fenetre=gtk_window_new(GTK_WINDOW_TOPLEVEL);  
         
	gtk_window_set_title(GTK_WINDOW(fenetre), "Connection ipmon");  
	gtk_window_set_position(GTK_WINDOW(fenetre), GTK_WIN_POS_CENTER);  
	gtk_window_set_default_size(GTK_WINDOW(fenetre), LARGEUR_FENETRE, HAUTEUR_FENETRE);  
	
	background = gtk_image_new_from_file("./images/ipmon.png");
	layout = gtk_layout_new(NULL, NULL);
  	gtk_container_add (GTK_CONTAINER (fenetre), layout);
 	
 	/*On initialise les boutons*/
	bouton_connect = gtk_button_new_with_label ("Connexion");
	bouton_enregistrer = gtk_button_new_with_label ("S'enregistrer");
	bouton_quitter = gtk_button_new_from_stock (GTK_STOCK_QUIT);
	champ_login  = gtk_entry_new();
	champ_pass  = gtk_entry_new();
	
	/* Options de la Zone de saisie champ_login*/
   	gtk_entry_set_max_length(GTK_ENTRY(champ_login), 50);          /* Nombre max de caractères */
    gtk_entry_set_text (GTK_ENTRY(champ_login), "Pseudo" ); /* Texte par défaut */
    gtk_entry_set_visibility(GTK_ENTRY(champ_login), TRUE);             /* Visibilité des caracteres */
    gtk_entry_set_editable(GTK_ENTRY(champ_login), TRUE);               /* Ecriture dans la zone possbile */
    
    /* Options de la Zone de saisie champ_pass*/
   	gtk_entry_set_max_length(GTK_ENTRY(champ_pass), 50);         
    gtk_entry_set_text (GTK_ENTRY(champ_pass), "Pass" ); 
    gtk_entry_set_visibility(GTK_ENTRY(champ_pass), FALSE);            
    gtk_entry_set_editable(GTK_ENTRY(champ_pass), TRUE);             
	
	/*on ajoute les boutons au container*/
	
	x_bouton = LARGEUR_FENETRE/2 - 300/2;
	gtk_layout_put(GTK_LAYOUT(layout), background, 0, 0);
	gtk_layout_put(GTK_LAYOUT(layout), champ_login, x_bouton, 250);
	gtk_layout_put(GTK_LAYOUT(layout), champ_pass, x_bouton+150, 250);
	gtk_layout_put(GTK_LAYOUT(layout), bouton_connect, x_bouton, 300);
	gtk_layout_put(GTK_LAYOUT(layout), bouton_enregistrer, x_bouton, 350);
	gtk_layout_put(GTK_LAYOUT(layout), bouton_quitter, x_bouton, 400);
	
	/*On redimentionne les boutons*/
	gtk_widget_set_size_request(champ_login, 150, 35);
	gtk_widget_set_size_request(champ_pass, 150, 35);
	gtk_widget_set_size_request(bouton_connect, 300, 35);
	gtk_widget_set_size_request(bouton_enregistrer, 300, 35);
	gtk_widget_set_size_request(bouton_quitter, 300, 35);
    	
	login->champ_login = champ_login;
	login->champ_pass = champ_pass;
	login->fenetre = fenetre;
	login->connect = 0;
      
	gtk_widget_show_all(fenetre);  
 	g_signal_connect(bouton_enregistrer, "clicked", G_CALLBACK(bouton_enregistrer_clicked), login);
 	g_signal_connect(bouton_connect, "clicked", G_CALLBACK(bouton_connect_clicked), login);
	g_signal_connect (bouton_quitter, "clicked", G_CALLBACK (cb_quit), NULL); 
	
	gtk_main();
	return login->connect;
}
      
int main(int argc, char *argv[])  {
	SDL_Init(SDL_INIT_VIDEO);
	dresseur = malloc(sizeof(Dresseur));
	int s_cli, connect = 0, end = 0,n;
	char buf[80];
	bzero(buf,80);

	if(argc == 3 && argv[1] != NULL && argv[2]!= NULL){
		s_cli = connect_serv_Ipmon(argv[1],strtol(argv[2],NULL,10));
		if(s_cli == -1){
			printf("Connection between client and server failed ! IP<%s> Port<%s>\n", argv[1], argv[2]);
			return 1;
		}
	}else{
		printf("Usage: clt_ipmon address port\n");
		return 0;
	}
	socket_cli = s_cli;
	
	gtk_init(NULL, NULL);
	
	connect = menu_gtk();
	
	if(connect == 1 && dresseur != NULL && dresseur->pseudo != NULL){
        jeu(socket_cli, dresseur);
	}

	send(socket_cli,"endclose",strlen("endclose"),0);
	recv(socket_cli,buf,80,0);
	bzero(buf,80);
	send(socket_cli,"999CLOSE",strlen("999CLOSE"),0);
	while(end == 0 &&(n = recv(socket_cli,buf,80,0))){
		if(strcmp(buf,"OKCLOSE")==0)
			end=1;
		bzero(buf,80);
	}
    printf("CLOSE !!");

	return 0;  
}  
