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

int main()
{
    int status, network_socket;
    pthread_t tid;
    struct sigaction *sa;

    /*sa->sa_handler = &disconnect_client;

    if (sigaction(SIGINT,sa,NULL) < 0)
    {
        printf("Error assigning disconnect handler\n");
        exit(1);
    }*/

    signal(SIGINT, disconnect_client);

    network_socket = setup_socket();

    if ((tid = pthread_create(&tid,NULL,listen_to_server,(void *)&network_socket)) < 0)
    {
        printf("Error creating thread\n");
        exit(0);
    }

    print_header();

    menu(tid,network_socket);

    pthread_join(tid,NULL);

    return 0;
}
