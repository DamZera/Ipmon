#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <malloc.h>
#include <gtk/gtk.h>

#include "protocol.h"

typedef struct Login{
    GtkWidget *champ_login;
    GtkWidget *champ_pass;
    GtkWidget *fenetre;
    int connect;
}Login;

typedef struct Message{
    char code [4];
    char data [100];
}Message;

typedef struct Ipmon{
    int id;
    char* nom;
    char* etat;
    char* type;
    int typeEntier; //Valeur du type d'ipmon : intervient dans le calcul du succès ou non d'une attaque
    int pv; //Point de vie
    int agilite; //Rapidité de reaction d'un ipmon
    int niveau; //Niveau de l'ipmon : intervient dans le calcul du degat subit par un ipmon [1 - 100]
    int puissance_attaque; //Puissance d'attaque simple
    char* nom_attaque;
    int precision_attaque;//Précision de l'attaque simple
    int puissance_defense; //Puissance de defense : intervient dans le calcul du degat subit par un ipmon
    char* nom_defense;
    int esquive; //Capacité d'esquive d'un ipmon : intervient dans le calcul du succès ou non d'une attaque = 100
    int precision; //Précision de l'ipmon
    int puissance_attaque_special;//Puissance de l'attaque spéciale d'un ipmon
    int precision_attaque_special;//Précision de l'attaque spéciale d'un ipmon
    char* nom_attaque_special;
    int puissance_defense_special;//Puissance de la défense spéciale d'un ipmon : intervient lorsqu'un ipmon est attaqué par une attaque spéciale
    char* nom_defense_special;
    struct Ipmon* next;
}Ipmon;

typedef struct Dresseur{
    char* id;
    char pseudo[100];
    char map[100];
    int niveau;
    int sock;
    int coodX;
    int coodY;
    struct Dresseur* next;
    Ipmon *ipmons;
    int nombreIpmons;
}Dresseur;

gboolean cb_quit (GtkWidget *p_widget, gpointer user_data);
gboolean bouton_connect_clicked(GtkWidget *widget, gpointer data);
void bouton_enregistrer_clicked(GtkWidget *widget, gpointer data);


int menu_gtk();
int connect_serv_Ipmon(char* adresse, int port);
void insert_dresseur(int *s_cli);
int connection_dresseur(int *s_cli);
Ipmon* ajouterIpmon(Ipmon* ipmon_list,int id,char* nom,char* etat,char* type,int typeEntier,int pv,int agilite,int niveau,int puissance_attaque,char* nom_attaque,int precision_attaque,int puissance_defense,int esquive,int precision,int puissance_attaque_special,int precision_attaque_special,char* nom_attaque_special,int puissance_defense_special);

#endif 
