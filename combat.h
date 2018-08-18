/*
** combat.h
**
** Système de combat tour par tour.
**
*/

# include <stdio.h>
# include <stdlib.h>
# include <time.h>
# include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include <gtk/gtk.h>

#define LARGEUR_FENETRE 600
#define HAUTEUR_FENETRE 600

typedef struct Ipmon {
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
}Ipmon;

typedef struct Dresseur {
	char* id;
	char* pseudo;
	int niveau;
	int sock;
	int coodX;
	int coodY;
	Ipmon ipmons [1000];
	int nombreIpmons;
} Dresseur;

typedef struct Parametres {
	int pv1;
	int pv2;
	int gagnant;
} Parametres;

typedef struct Equipe {
	Dresseur dresseur1;
	Dresseur dresseur2;
} Equipe;

typedef struct Param{
	GtkWidget *champ_texte;
	GtkWidget *fenetre;
}Param;

typedef struct Donnees {
	Dresseur dresseur;
	Ipmon ipmon;
	GtkWidget *champ_texte;
	GtkWidget *label;
}Donnees;

Ipmon chercherIpmonDeDresseur (Dresseur dresseur, int id);
int chercherIpmonIndiceDeDresseur (Dresseur dresseur, int id);
int Combat2Dresseurs (Dresseur dresseur1, Dresseur dresseur2) ;
int CombatDresseurIpmon (Dresseur dresseur, Ipmon ipmon);
int CombatEquipe (Equipe equipe1, Equipe equipe2);
int attaque (Ipmon* ipmon1, Ipmon* ipmon2);
int attaque_speciale (Ipmon* ipmon1, Ipmon* ipmon2);
void viderBuffer() ;
void EchangerIpmons (Dresseur *dresseur1, Dresseur *dresseur2, Ipmon ipmonDresseur1, Ipmon ipmonDresseur2);
Dresseur* Ajouter_Ipmon_Dresseur (Dresseur *dresseur1, Ipmon ipmon);