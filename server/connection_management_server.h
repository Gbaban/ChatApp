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


void *handle_connection(void *vargp);
void manage_multiple_connections(int server_socket);


#endif // CONNECTION_MANAGEMENT_SERVER_H_INCLUDED
