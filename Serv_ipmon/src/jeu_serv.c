#include "jeu_serv.h"


//void combat_init(MYSQL* ipmon_bdd,char* msg,int *s_dial){
    // Ipmon* ipmon_adv  = chercher_ipmon(ipmon_bdd,strtol(msg,NULL,10));
    // Ipmon* mon_ipmon;
    // Dresseur* dresseur = chercher_dresseur(dresseur_list_jeu,*s_dial);
    // char buf[BUFFER_SIZE];

    // /* INITIALISATION des pokemons */
    // printf("combat");
    // send(*s_dial,"msg:1",strlen("msg:1"),0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // bzero(buf,BUFFER_SIZE);
    // send(*s_dial,"msg:2",strlen("msg:2"),0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // bzero(buf,BUFFER_SIZE);
    // sprintf(buf,"%d:%s:%s:%s:%d",ipmon_adv->id,ipmon_adv->nom,ipmon_adv->etat,ipmon_adv->type,ipmon_adv->typeEntier);
    // send(*s_dial,buf,BUFFER_SIZE,0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // bzero(buf,BUFFER_SIZE);
    // sprintf(buf,"%d:%d:%d:%d:%s",ipmon_adv->pv,ipmon_adv->agilite,ipmon_adv->niveau,ipmon_adv->puissance_attaque,ipmon_adv->nom_attaque);
    // send(*s_dial,buf,BUFFER_SIZE,0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // bzero(buf,BUFFER_SIZE);
    // sprintf(buf,"%d:%d:%d:%d",ipmon_adv->precision_attaque,ipmon_adv->puissance_attaque,ipmon_adv->esquive,ipmon_adv->precision);
    // send(*s_dial,buf,BUFFER_SIZE,0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // bzero(buf,BUFFER_SIZE);
    // sprintf(buf,"%d:%d:%s:%d",ipmon_adv->puissance_attaque_special,ipmon_adv->precision_attaque_special,ipmon_adv->nom_attaque_special,ipmon_adv->puissance_defense_special);
    // send(*s_dial,buf,BUFFER_SIZE,0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // mon_ipmon = chercher_ipmon(ipmon_bdd,strtol(buf,NULL,10));
    // bzero(buf,BUFFER_SIZE);
    // send(*s_dial,"end",strlen("end"),0);

    // if (mon_ipmon->agilite > ipmon_adv->agilite)
    //     send(*s_dial,"Le dresseur commence le jeu\n",strlen("Le dresseur commence le jeu\n"),0);
    // else
    //     send(*s_dial,"L'ipmon commence le jeu\n",strlen("L'ipmon commence le jeu\n"),0);

    // // STUBED COMBAT PART 
//}

/*
Ipmon* chercher_ipmon(MYSQL* ipmon_bdd,int id){
    Ipmon* ipmon = malloc(sizeof(Ipmon));
    char requete[150];
    MYSQL_RES *result;
    MYSQL_ROW row;

    sprintf(requete,"SELECT * FROM ipmon WHERE id_IPmon = '%d' ",id);
    LOG_DBG("requete = %s",requete);

    if(mysql_query(ipmon_bdd, requete)){
                //finish_with_error(ipmon_bdd);
    } 
    result = mysql_store_result(ipmon_bdd);
    row = mysql_fetch_row(result);
    ipmon->id = strtol(row[0],NULL,10);
    
    snprintf(ipmon->nom, MAX_SIZE_IPMON_STR, "%s", row[1]);
    snprintf(ipmon->etat, MAX_SIZE_IPMON_STR, "%s", row[2]);
    snprintf(ipmon->type, MAX_SIZE_IPMON_STR, "%s", row[3]);
    ipmon->typeEntier = strtol(row[4],NULL,10);
    
    ipmon->pv = strtol(row[5],NULL,10);
    ipmon->agilite = strtol(row[6],NULL,10);
    ipmon->niveau = 1;
    ipmon->puissance_attaque = strtol(row[7],NULL,10);
    snprintf(ipmon->nom_attaque, MAX_SIZE_IPMON_STR, "%s", row[8]);
    
    ipmon->precision_attaque = strtol(row[9],NULL,10);
    ipmon->puissance_defense = strtol(row[10],NULL,10);
    ipmon->esquive = strtol(row[11],NULL,10);
    ipmon->precision = strtol(row[12],NULL,10);
    
    ipmon->puissance_attaque_special = strtol(row[13],NULL,10);
    ipmon->precision_attaque_special = strtol(row[14],NULL,10);
    snprintf(ipmon->nom_attaque_special, MAX_SIZE_IPMON_STR, "%s", row[15]);
    ipmon->puissance_defense_special = strtol(row[16],NULL,10);

    return ipmon;
}*/
            
int combat(Ipmon ipmonDresseur, Ipmon ipmon, int *s_dial){
    // char buf[BUFFER_SIZE];
    // int choix;
    // int gagnant;
    // int id, i;
    
    // int tour = 1;
    // while ((ipmonDresseur.pv > 0) && (ipmon.pv > 0)) { //Tant que l'un des ipmons ne meurt pas
    // sprintf(buf,"Tour %d\n", tour);
    // send(*s_dial,buf,BUFFER_SIZE,0);
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // if (ipmonDresseur.agilite > ipmon.agilite) { //L'ipmon du dresseur est plus rapide que l'ipmon sauvage, donc c'est lui qui attaque en 1er
    
    // recv(*s_dial,buf,BUFFER_SIZE,0);
    // choix = strtol(buf,NULL,10);
    // if (choix == 1) {
    //     int x = ipmon.pv, y = ipmonDresseur.pv;
    //     ipmon.pv = attaque (&ipmonDresseur, &ipmon);
    //     sprintf(buf,"Vous avez fait l'attaque : %s\n", ipmonDresseur.nom_attaque);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     recv(*s_dial,buf,BUFFER_SIZE,0);
    //     if (x == ipmon.pv)
    //         send(*s_dial,"Attaque ratée !\n",strlen("Attaque ratée !\n"),0);
    //     else {
    //         if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
    //             gagnant = 1;
    //             break;
    //         }
    //         sprintf(buf,"Il lui reste maintenant %d pv\n", ipmon.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    //     int a = rand () % 2;
    //     if (a == 0) {
    //         sprintf(buf,"L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
    //         if (y == ipmonDresseur.pv){
    //             sprintf(buf,"L'attaque de l'ipmon sauvage est ratée !\n");
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }
    //         else {
    //             if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //                 gagnant = 2;
    //                 break;
    //             }
    //             sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }
    //     }
    //     else {
    //         sprintf(buf,"L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque_special);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
    //         if (y == ipmonDresseur.pv){
    //             sprintf(buf,"L'attaque spéciale de l'ipmon sauvage est ratée !\n");
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }else {
    //             if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //                 gagnant = 2;
    //                 break;
    //             }
    //             sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }
    //     }
    // }
    // else if (choix == 2) {
    //     int x = ipmon.pv, y = ipmonDresseur.pv;
    //     ipmon.pv = attaque_speciale (&ipmonDresseur, &ipmon);
    //     sprintf(buf,"Vous avez fait l'attaque spéciale : %s\n", ipmonDresseur.nom_attaque_special);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     if (x == ipmon.pv){
    //         sprintf(buf,"Attaque ratée !\n");
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }else {
    //         if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
    //             gagnant = 1;
    //             break;
    //         }
    //         sprintf(buf,"Il lui reste maintenant %d pv\n", ipmon.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    //     int a = rand () % 2;
    //     if (a == 0) {
    //         sprintf(buf,"L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque_special);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
    //         if (y == ipmonDresseur.pv){
    //             sprintf(buf,"L'attaque spéciale de l'ipmon sauvage est ratée !\n");
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }else {
    //             if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //                 gagnant = 2;
    //                 break;
    //             }
    //             sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }
    //     }
    //     else {
    //         sprintf(buf,"L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
    //         if (y == ipmonDresseur.pv){
    //             sprintf(buf,"L'attaque de l'ipmon sauvage est ratée !\n");
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }else {
    //             if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //                 gagnant = 2;
    //                 break;
    //             }
    //             sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //             send(*s_dial,buf,BUFFER_SIZE,0);
    //         }
    //     }
    // }
    // else if (choix == 0) {
    //     send(*s_dial,"4",strlen("4"),0);
    //     recv(*s_dial,buf,BUFFER_SIZE,0);
    //     char c = buf[0];
    //     if (c == 'y') {
    //         sprintf(buf,"Vous avez abandonné le combat, vous avez donc perdu\n");
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         gagnant = 3;
    //         break;
    //     }
        
    // }
    // else {
    //     sprintf(buf,"Entrez un nombre valide !");
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    // }
    // }
    // else {
    // int attaque_ipmon_sauvage = rand () % 2; //0 = attaque simple, 1 = Attaque spéciale
    // int x = ipmon.pv, y = ipmonDresseur.pv;
    // if (attaque_ipmon_sauvage == 0) {
    //     sprintf(buf,"L'ipmon sauvage a fait l'attaque : %s\n", ipmon.nom_attaque);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     ipmonDresseur.pv = attaque (&ipmon, &ipmonDresseur);
    //     if (y == ipmonDresseur.pv){
    //         sprintf(buf,"L'attaque de l'ipmon sauvage est ratée !\n");
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }else {
    //         if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //             gagnant = 2;
    //             break;
    //         }
    //         sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    // }
    // else {
    //     sprintf(buf,"L'ipmon sauvage a fait l'attaque spéciale : %s\n", ipmon.nom_attaque);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     ipmonDresseur.pv = attaque_speciale (&ipmon, &ipmonDresseur);
    //     if (y == ipmonDresseur.pv){
    //         sprintf(buf,"L'attaque spéciale de l'ipmon sauvage est ratée !\n");
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }else {
    //         if (ipmonDresseur.pv == -1) { //Si le dresseur n'a plus de vie
    //             gagnant = 2;
    //             break;
    //         }
    //         sprintf(buf,"Il vous reste maintenant %d pv\n", ipmonDresseur.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    // }

    // send(*s_dial,"3",strlen("3"),0);
    // if (choix == 1) {
    //     ipmon.pv = attaque (&ipmonDresseur, &ipmon);
    //     sprintf(buf,"Vous avez fait l'attaque : %s\n", ipmonDresseur.nom_attaque);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     if (x == ipmon.pv)
    //         sprintf(buf,"Attaque ratée !\n");
    //     else {
    //         if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
    //             gagnant = 1;
    //             break;
    //         }
    //         sprintf(buf,"Il lui reste maintenant %d pv\n", ipmon.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    // }
    // else if (choix == 2) {
    //     ipmon.pv = attaque_speciale (&ipmonDresseur, &ipmon);
    //     sprintf(buf,"Vous avez fait l'attaque spéciale : %s\n", ipmonDresseur.nom_attaque);
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     if (x == ipmon.pv){
    //         sprintf(buf,"Attaque ratée !\n");
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    //     }else {
    //         if (ipmon.pv == -1) { //Si l'ipmon sauvage n'a plus de vie
    //             gagnant = 1;
    //             break;
    //         }
    //         sprintf(buf,"Il lui reste maintenant %d pv\n", ipmon.pv);
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //     }
    // }
    // else if (choix == 0) {
    //     send(*s_dial,"4",strlen("4"),0);
    //     recv(*s_dial,buf,BUFFER_SIZE,0);
    //     char c = buf[0];
    //     if (c == 'y') {
    //         sprintf(buf,"Vous avez abandonné le combat, vous avez donc perdu\n");
    //         send(*s_dial,buf,BUFFER_SIZE,0);
    //         gagnant = 3;
    //         break;
    //     }
        
    // }
    // else {
    //     sprintf (buf,"Entrez un nombre valide !");
    //     send(*s_dial,buf,BUFFER_SIZE,0);
    // }
    // }
    // tour++;
    // }
     
     // STUBED COMBAT PART 
    return 0;
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
            
            
            
            
            
            