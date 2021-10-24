#include "main.h"
#include <SDL/SDL.h>

#define LARGEUR_FENETRE 640
#define HAUTEUR_FENETRE 480
#define DEBUG 1

Dresseur *dresseur;

void printManual(char* cmd)
{
    printf("%s <ip_server> <port_server> <port_client> <cmd>\n\n", cmd);
    printf("Client must be launch in the root folder\n");
    printf("Parameters:\n");
    printf("\tip_server : IP of the IPMON server\n");
    printf("\tport_server : port of the server\n");
    printf("\tport_client : port IN of the client\n");
    printf("\tcmd : support connection and register\n");
    printf("\t\t c:<pseudo>:<pass> : connection with pseudo and pass\n");
    printf("\t\t r:<pseudo>:<pass> : register to IPMON with pseudo and pass\n");
}

int connectToIpmonServer(int port)
{
    LOG_DBG("Start connectToIpmonServer");
    int s_cli;
    int err;
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100000;

    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl (INADDR_ANY);
    serv_addr.sin_port = htons (port);
    memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

    s_cli = socket (AF_INET, SOCK_DGRAM, 0);
    // Timeout on receive
    if (setsockopt(s_cli, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        perror("Error");
    }

    err = bind(s_cli, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(err == -1)
    {
        LOG_ERR("Bind failed for socket %d", s_cli);
        return -1;
    }

    return s_cli;
}

void registerPlayer(Login* login)
{
    char buf [BUFFER_SIZE];
    bzero(buf, BUFFER_SIZE);

    snprintf(buf, 200, "%d:%s:%s", REGISTER_TO_IPMON, login->pseudo, login->pass);
    sendto(login->socket, buf, strlen(buf), MSG_CONFIRM,
        (const struct sockaddr *) login->srvaddr, sizeof(*(login->srvaddr)));
}

int connectPlayerToIPMON(Login* login)
{
    char buffer [BUFFER_SIZE];
    char *token, *string, *pstring;
    int n ;

    struct sockaddr_in rcvaddr;
    memset(&rcvaddr, 0, sizeof(rcvaddr));

    int len;
    len = sizeof(rcvaddr);  //len is value/resuslt
    
    bzero(buffer, 200);
    snprintf(buffer, 200, "%d:%s:%s", CONNECT_TO_IPMON, login->pseudo, login->pass);
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
            
            sprintf(dresseur->pseudo, "%s", login->pseudo);
            login->connect = 1;

            LOG_INFO("LOGIN ok");
        }
        // Connection KO
        else if (code == REFUSE_CONNECTION)
        {
            LOG_INFO("LOGIN error");
        }
    }

    free(pstring);

    if(login->connect == 1){
        return 0;
    }

    return 1;
}

int processCommand(int socket, struct sockaddr_in* srvaddr, char* cmd) 
{
    LOG_DBG("Start processCommand");
    char* string, *token, *cmdTmp;
    Login login;

    login.socket = socket;
    login.connect = 0;
    login.srvaddr = srvaddr;

    string = strdup(cmd);
    
    // Parse command
    if ((cmdTmp = strsep(&string, ":")) != NULL)
    {
        if ((token = strsep(&string, ":")) != NULL)
        {
            strncpy(login.pseudo, token, 50);
        }

        if ((token = strsep(&string, ":")) != NULL)
        {
            strncpy(login.pass, token, 50);
        }

        if (strcmp(cmdTmp, "c") == 0)
        {
            connectPlayerToIPMON(&login);
        } 
        else if (strcmp(cmdTmp, "r") == 0)
        {
            registerPlayer(&login);
        }
        else
        {
            LOG_WARN("Cmd unknown %s", cmd);
        }
    }
    return login.connect;
}
      
int main(int argc, char *argv[])
{
    dresseur = malloc(sizeof(Dresseur));
    int s_cli, connect = 0, end = 0,n;
    char buf[BUFFER_SIZE];
    bzero(buf,BUFFER_SIZE);

    struct sockaddr_in serv_addr;

    if(argc == 5 
        && argv[1] != NULL 
        && argv[2] != NULL
        && argv[3] != NULL
        && argv[4] != NULL)
    {
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr (argv[1]);
        serv_addr.sin_port = htons (strtol(argv[2],NULL,10));
        memset (&serv_addr.sin_zero, 0, sizeof(serv_addr.sin_zero));

        int portClient = strtol(argv[3],NULL,10);

        s_cli = connectToIpmonServer(portClient);
        if(s_cli == -1)
        {
            LOG_ERR("Connection between client and server failed ! IP<%s> portServer<%s> portClient<%s>",
                argv[1], argv[2], argv[3]);
            return 1;
        }
    }
    else
    {
        printManual(argv[0]);
        return 0;
    }

    connect = processCommand(s_cli, &serv_addr, argv[4]);
    
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
