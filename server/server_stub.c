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
#include <signal.h>


#include "utility_server.h"
#include "connection_management_server.h"

#define PORT 9002
#define ADDRESS "192.168.0.102"

void INThandler(int sig)
{

  close_all_connections();
  printf("Closing server\n");
  exit(0);

}


void print_header()
{
    printf("+**********************************************************************+\n");
    printf("*                                 CHAT APP                             *\n");
    printf("*                          Best Server VERSION EVER                    *\n");
    printf("+**********************************************************************+\n\n\n");
}


int main(int argc,char *argv[])
{
    print_header();
    signal(SIGINT, INThandler);
    logedin_user_dimension=0;


    //create the server socket
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    ////////////////////////////////////////////////////////////////

    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;

    //setare port
    char * port=get_ip_port(argc,argv,GET_PORT);
    if(port!=NULL)
        server_address.sin_port = htons(atoi(port));
    else
        server_address.sin_port=htons(PORT);


    //setare ip
    char * ip=get_ip_port(argc,argv,GET_IP);
    if(ip!=NULL)
        server_address.sin_addr.s_addr=inet_addr(ip);
    else
        server_address.sin_addr.s_addr = INADDR_ANY;
    /////////////////////////////////////////////////////////////////

    //bind the server socket to the address and port
    if (bind(server_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf(ANSI_COLOR_RED     "[main]Error binding server socket"     ANSI_COLOR_RESET "\n");
        exit(0);
    }
    ////////////////////////////////////////////////////////////////////////////////////


    //listen for connections
    if (listen(server_socket,5) < 0)
    {
        printf(ANSI_COLOR_RED     "[main]Error in listening server socket"     ANSI_COLOR_RESET "\n");
        exit(0);
    }
    ////////////////////////////////////////////////////////////////////////////////



    //accept the connection from the client socket
    manage_multiple_connections(server_socket);
    ///////////////////////////////////////////////////////////////////////////////////


    return 0;
}
