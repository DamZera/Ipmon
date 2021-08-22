#include "mysql_ipmon.h"

/*Fonction qui retourne un pointeur MYSQL qui permet de communiquer avec la BDD*/
MYSQL *connect_bdd(){
    MYSQL* mysql;
    /*Initialisation de MySQL*/
    mysql = mysql_init(NULL);
    /*Options de connexion*/
    mysql_options(mysql,MYSQL_READ_DEFAULT_GROUP,"option");

    if(mysql_real_connect(mysql,
    		"localhost", //IP de la bdd
    		"ipmon", 	 //User
    		"ipmon2021", 		 //Pass
    		"ipmon", 	 //Bdd
    		8889,0,0)){
    	printf("Connexion sucess !!\n");
    }
    else {
    	printf("Error in connect_bdd()\n");
    	printf( "Error: %s\n", mysql_error( mysql ) ) ;
    }
    return mysql;
}


void finish_with_error(MYSQL *con){
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  exit(1);        
}