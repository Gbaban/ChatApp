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


#include "utility_server.h"
#include "connection_management_server.h"

#define PORT 9002
#define ADDRESS "192.168.0.102"




int main(int argv,char *argc[])
{

    //create the server socket
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    ////////////////////////////////////////////////////////////////

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;

    //setare port
    char * port=get_ip_port(argv,argc,GET_PORT);
    if(port!=NULL)
        server_address.sin_port = htons(atoi(port));
    else
        server_address.sin_port=htons(PORT);


    //setare ip
    char * ip=get_ip_port(argv,argc,GET_IP);
    if(ip!=NULL)
        server_address.sin_addr.s_addr=inet_addr(ip);
    else
        server_address.sin_addr.s_addr = INADDR_ANY;
    /////////////////////////////////////////////////////////////////

    //bind the server socket to the address and port
    if (bind(server_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf("Error binding server socket\n");
        exit(0);
    }
    ////////////////////////////////////////////////////////////////////////////////////


    //listen for connections
    if (listen(server_socket,5) < 0)
    {
        printf("Error in listening server socket\n");
        exit(0);
    }
    ////////////////////////////////////////////////////////////////////////////////


    //accept the connection from the client socket
    manage_multiple_connections(server_socket);
    ///////////////////////////////////////////////////////////////////////////////////


    return 0;
}
