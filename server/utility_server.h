#ifndef UTILITY_SERVER_H_INCLUDED
#define UTILITY_SERVER_H_INCLUDED

#include <string.h>
#include<stdio.h>
#include<stdlib.h>
#include "connection_management_server.h"


#define GET_PORT 1
#define GET_IP 2

#define IS_REGISTERED 1
#define IS_NOT_REGISTERED 0

char * get_ip_port(int argv,char *argc[],int optiune);
char ** extract_user_name_password(const char *client_response_smth,int n);
int isInFile(char **name_password);
int login(const char *client_response_smth,int n);
int signup(const char *client_response_smth,int n);
char *pack_message(char *original_message, char flags);
int command(const char *client_response_smth,int client_socket);
int messageInterpreter(const char client_header[2], const char *client_response_smth, int client_socket);

#endif // UTILITY_SERVER_H_INCLUDED
