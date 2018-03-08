#ifndef CONNECTION_MANAGEMENT_SERVER_H_INCLUDED
#define CONNECTION_MANAGEMENT_SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define MESSAGE	0x80
#define LOGIN	0x40
#define SIGNUP 0x20
#define COMMAND 0x10

#define FAIL 0
#define LOGIN_SUCCESS 1
#define LOGIN_FAIL 2
#define SIGNUP_SUCCESS 3
#define SIGNUP_FAIL 4
#define SUCCESS 254

typedef struct{
  char name[31];
  int socket;
}client_t;

client_t logedin_user_sockets[1000];
int logedin_user_dimension;


void *handle_connection(void *vargp);
void manage_multiple_connections(int server_socket);
void close_all_connections();

#endif // CONNECTION_MANAGEMENT_SERVER_H_INCLUDED
