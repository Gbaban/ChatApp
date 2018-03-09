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

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char * get_ip_port(int argv,char *argc[],int optiune);
char ** extract_user_name_password(const char *client_response_smth,int n);
int isInFile(char **name_password);
int login(const char *client_response_smth,int n);
int signup(const char *client_response_smth,int n);
char *pack_message(char *original_message,unsigned char flags, const char *sender);
int command(const char *client_response_smth,int client_socket);
int messageInterpreter(const char client_header[2], const char *client_response_smth, int client_socket);
char *getUsernameBySocket(int client_socket);

#endif // UTILITY_SERVER_H_INCLUDED
