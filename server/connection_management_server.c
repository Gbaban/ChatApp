#include "connection_management_server.h"


int client_sockets[100];
int client_sockets_dimension=0;
int nr=0;//doar verificare sa vad daca functioneaza bine



///////////////////////////////////////////////////////////////////////////////////////////////////

void *handle_connection(void *vargp)
{

    nr++;
    char server_message[256] = "You have reached the server!";
    int client_socket=*((int *)vargp);
    char client_response[256];
    if (recv(client_socket,client_response,sizeof(client_response),0) < 0)
    {
            printf("Error recieving data from clientn");
            exit(1);
    }


    //trimite raspunsul la toate socket-urile---de avut grija cand clientul va functiona
    //sa nu mai trimitem inapoi mesajul si acelui client care l-a trimis
    //ca va fi afisat in consola lui de 2 ori
    printf("The client sent the following data here: %s %d\n",client_response,nr);
    sprintf(server_message,"%s %d\n",server_message,nr);
    int i=0;
    for(;i<client_sockets_dimension;i++)
            send(client_sockets[i],server_message,sizeof(server_message),0);
        ////////////////////////////////////////////////////////////////////////////

    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void manage_multiple_connections(int server_socket)
{

    int client_socket;

    while(client_socket = accept(server_socket,NULL,NULL))
    {

        struct sockaddr_in client_address;

        if (client_socket < 0)
        {
            printf("Error accepting client socket\n");
            exit(0);
        }

        //pastrarea tuturor clientilor
        client_sockets[client_sockets_dimension]=client_socket;
        client_sockets_dimension++;
        /////////////////////////////////////////////////////////////


        //initializare threads
        pthread_t tid;
        pthread_create(&tid, NULL, handle_connection, (void *)&client_socket);
        pthread_join(tid, NULL);
        //////////////////////////////////////////////////////////////////

    }//loop for incoming connections
    close(server_socket);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////