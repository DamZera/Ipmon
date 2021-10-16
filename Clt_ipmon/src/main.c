#include "main.h"
#include <SDL/SDL.h>

#define LARGEUR_FENETRE 640
#define HAUTEUR_FENETRE 480
#define DEBUG 1

Dresseur *dresseur;

gboolean cb_quit (GtkWidget *p_widget, gpointer user_data)
{
    gtk_main_quit();
    return FALSE;
}

int connectToIpmonServer(int port)
{
    int s_cli;
    int err;

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_addr.sin_port = htons (port);
    memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    s_cli = socket (AF_INET, SOCK_DGRAM, 0);
    err = bind(s_cli, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(err == -1)
    {
        LOG_ERR("Bind failed for socket %d", s_cli);
        return -1;
    }

    return s_cli;
}

void bouton_enregistrer_clicked(GtkWidget *widget, gpointer data){
    Login *login = data;
    char pseudo[50];
    char pass[50];    
    char buf [200];
    bzero(buf, 200);

    strcpy(pseudo,gtk_entry_get_text(GTK_ENTRY(login->champ_login)));
    strcpy(pass,gtk_entry_get_text(GTK_ENTRY(login->champ_pass)));

    snprintf(buf, 200, "%d:%s:%s", REGISTER_TO_IPMON, pseudo, pass);
    sendto(login->socket, buf, strlen(buf), MSG_CONFIRM,
        (const struct sockaddr *) login->srvaddr, sizeof(*(login->srvaddr)));
}

gboolean bouton_connect_clicked(GtkWidget *widget, gpointer data){
    Login *login = (Login *)data;
    char buffer [200];
    char *token, *string, *pstring;
    char pseudo[50];
    char pass[50];    
        
    int n ;


    struct sockaddr_in rcvaddr;
    memset(&rcvaddr, 0, sizeof(rcvaddr));

    int len;
    len = sizeof(rcvaddr);  //len is value/resuslt
    
    // TODO strncpy more secure
    strcpy(pseudo,gtk_entry_get_text(GTK_ENTRY(login->champ_login)));
    strcpy(pass,gtk_entry_get_text(GTK_ENTRY(login->champ_pass)));
    
    bzero(buffer, 200);
    snprintf(buffer, 200, "%d:%s:%s", CONNECT_TO_IPMON, pseudo, pass);
    sendto(login->socket, buffer, strlen(buffer), MSG_CONFIRM,
        (const struct sockaddr *) login->srvaddr, sizeof(*(login->srvaddr)));

    bzero(buffer, 200);
    n = recvfrom(login->socket, (char *)buffer, 200, 
                MSG_WAITALL, ( struct sockaddr *) &rcvaddr, &len);
    LOG_INFO("message: %s ", buffer);
    
    pstring = string = strdup(buffer);
    
    // Parse login message
    if ((token = strsep(&string, ":")) != NULL)
    {
        int code = strtol(token, NULL, 10);
        // Connection OK
        if (code == ACCEPT_CONNECTION)
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

            LOG_INFO("LOGIN ok");
        }
        // Connection KO
        else if (code == REFUSE_CONNECTION)
        {
            gtk_entry_set_text(GTK_ENTRY(login->champ_login),"Login error");
            LOG_INFO("LOGIN error");
        }
    }

    free(pstring);

    if(login->connect == 1){
        //gtk_widget_destroy(login->fenetre);
        LOG_INFO("CB destroy");
        gtk_main_quit ();
        return FALSE;
    }

    return TRUE;
}

int menu_gtk(int socket, struct sockaddr_in* srvaddr){
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
    gtk_entry_set_text (GTK_ENTRY(champ_login), "dressA" ); /* Texte par défaut */
    gtk_entry_set_visibility(GTK_ENTRY(champ_login), TRUE);             /* Visibilité des caracteres */
    //gtk_entry_set_editable(GTK_ENTRY(champ_login), TRUE);               /* Ecriture dans la zone possbile */
    
    /* Options de la Zone de saisie champ_pass*/
    gtk_entry_set_max_length(GTK_ENTRY(champ_pass), 50);         
    gtk_entry_set_text (GTK_ENTRY(champ_pass), "1234" ); 
    gtk_entry_set_visibility(GTK_ENTRY(champ_pass), FALSE);            
    //gtk_entry_set_editable(GTK_ENTRY(champ_pass), TRUE);             
    
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
    login->socket = socket;
    login->srvaddr = srvaddr;
    login->connect = 0;
      
    gtk_widget_show_all(fenetre);  
    g_signal_connect(bouton_enregistrer, "clicked", G_CALLBACK(bouton_enregistrer_clicked), login);
    g_signal_connect(bouton_connect, "clicked", G_CALLBACK(bouton_connect_clicked), login);
    g_signal_connect (bouton_quitter, "clicked", G_CALLBACK (cb_quit), NULL); 
    
    gtk_main();
    return login->connect;
}
      
int main(int argc, char *argv[])
{
    dresseur = malloc(sizeof(Dresseur));
    int s_cli, connect = 0, end = 0,n;
    char buf[BUFFER_SIZE];
    bzero(buf,BUFFER_SIZE);

    struct sockaddr_in serv_addr;

    if(argc == 4 && argv[1] != NULL && argv[2]!= NULL){
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr (argv[1]);
        serv_addr.sin_port = htons (strtol(argv[2],NULL,10));
        memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

        s_cli = connectToIpmonServer(strtol(argv[3],NULL,10));
        if(s_cli == -1)
        {
            LOG_ERR("Connection between client and server failed ! IP<%s> portServer<%s> port<%s>",
                argv[1], argv[2], argv[3]);
            return 1;
        }
    }
    else
    {
        LOG_ERR("Usage: clt_ipmon address port");
        return 0;
    }
    
    gtk_init(NULL, NULL);
    
    connect = menu_gtk(s_cli, &serv_addr);

    SDL_Init(SDL_INIT_VIDEO);
    
    if(connect == 1 && dresseur != NULL && dresseur->pseudo != NULL){
        jeu(s_cli, &serv_addr, dresseur);
    }

    snprintf(buf, BUFFER_SIZE, "%d", CLOSE_CLIENT);
    send(s_cli, buf, strlen(buf), 0);
    sleep(1); // wait last message
    shutdown(s_cli, SHUT_RDWR);
    close(s_cli);
    LOG_ERR("CLOSE !!");

    return 0;  
}  
