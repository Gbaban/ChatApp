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
#define COMMAND 0x00


void *handle_connection(void *vargp);
void manage_multiple_connections(int server_socket);
void close_all_connections();
char *pack_message(char *original_message, char flags);
void close_all_connections();


#endif // CONNECTION_MANAGEMENT_SERVER_H_INCLUDED
