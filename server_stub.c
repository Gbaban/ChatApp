#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>

#define PORT 9002

int main()
{
    char server_message[256] = "You have reached the server!";
    
    //create the server socket
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);
    
    //define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;
    
    //bind the server socket to the address and port
    if (bind(server_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf("Error binding server socket\n");
        exit(0);
    }
    
    //listen for connections
    if (listen(server_socket,5) < 0)
    {
        printf("Error in listening server socket\n");
        exit(0);
    }
    
    //accept the connection from the client socket
    int client_socket;
    struct sockaddr_in client_address;
    client_socket = accept(server_socket,NULL,NULL);
    if (client_socket < 0)
    {
        printf("Error accepting client socket\n");
        exit(0);
    }
    
    char client_response[256];
    if (recv(client_socket,client_response,sizeof(client_response),0) < 0)
    {
        printf("Error recieving data from client\n");
        exit(1);
    }
    
    //print out the server's response
    printf("The client sent the data: %s\n",client_response);

    send(client_socket,server_message,sizeof(server_message),0);
    close(server_socket);
    return 0;
}
