#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum 
{
    CONNECT_TO_IPMON          = 0,
    REGISTER_TO_IPMON         = 2,
    NEW_COORDINATES           = 4,
    COMBAT                    = 6,
    CLOSE_CLIENT              = 8
} client_code_msg;

typedef enum 
{
    ACCEPT_CONNECTION         = 1,
    REFUSE_CONNECTION         = 3,
    ACCEPT_REGISTER           = 5,
    REFUSE_REGISTER           = 7
} server_code_msg;

#endif