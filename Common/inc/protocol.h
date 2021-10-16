#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum 
{
    // O is use by strtol in case of error !
    CONNECT_TO_IPMON          = 2,
    REGISTER_TO_IPMON         = 4,
    NEW_COORDINATES           = 6,
    COMBAT                    = 8,
    SEND_LIST_PLAYERS         = 10,
    CLOSE_CLIENT              = 12
} client_code_msg;

typedef enum 
{
    ACCEPT_CONNECTION         = 1,
    REFUSE_CONNECTION         = 3,
    ACCEPT_REGISTER           = 5,
    REFUSE_REGISTER           = 7
} server_code_msg;

#define BUFFER_SIZE 200
#define BIG_BUFFER_SIZE 2048

#endif