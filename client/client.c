#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "connection_utils.h"
#include "app_utils.h"

#define PORT 9002

int main(int argc, char *argv[])
{
    int status, network_socket;
    pthread_t tid;

    char * port=get_ip_port(argc,argv,GET_PORT);
    if(port!=NULL)
        network_socket = setup_socket(atoi(port));
    else
        network_socket = setup_socket(PORT);

    disconect_socket = network_socket;

    signal(SIGINT, disconnect_client);



    if ((tid = pthread_create(&tid,NULL,listen_to_server,(void *)&network_socket)) < 0)
    {
        printf(ANSI_COLOR_RED     "[main]Error creating thread"     ANSI_COLOR_RESET "\n");
        exit(0);
    }

    print_header();

    menu(tid,network_socket);

    pthread_join(tid,NULL);

    return 0;
}
