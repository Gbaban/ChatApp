#include "connection_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDRESS "192.168.0.102"
#define PORT 9002

int setup_socket()
{
    int network_socket;
    network_socket = socket(AF_INET,SOCK_STREAM,0);
    if (!network_socket)
    {
        printf("Error creating socket in client\n");
        exit(0);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    //server_address.sin_addr.s_addr = inet_addr(ADDRESS); //use when we actually get an IP
    server_address.sin_addr.s_addr = INADDR_ANY; //use for debug purposes

    if (connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf("Error establishing connection in client socket\n");
        exit(0);
    }

    //beta handshaking
    char message[] = "Sup, I connected!\n";
    send(network_socket,message,sizeof(message),0);

    char server_response[256];
    if (recv(network_socket,server_response,sizeof(server_response),0) < 0)
    {
        printf("Error recieving data from server\n");
        exit(1);
    }

    //print out the server's response
    printf("The server sent the data: %s\n",server_response);
    
    return network_socket;
}

void server_message_interpreter(char *server_message)
{
    printf("Recieved from server: %s\n",server_message);
    //TODO based on protocol, interepret message from server and act accordingly
	
	char message_length[1];
	message_length[0] = server_message[0];
	
	char header_data[1];
	header_data[0] = server_message[1];
	
	char *body_message = (char *) malloc(256);
	body_message = server_message + 2;
	printf("%s\n", body_message);
	
	if(strlen(body_message) != message_length[0]){
		printf("Some bits got lost\n");
		exit(3);
	}

	if(header_data[0] == 0b00000000){
		printf("comanda\n");
	}
	if(header_data[0] == 0b10000000){
		printf("mesaj\n");
	}
	if(header_data[0] == 0b01000000){
		printf("login\n");
	}
	if(header_data[0] == 0b00100000){
		printf("signup\n");
	}
}

void* listen_to_server(void *void_arg)
{
    char server_message[256];
    
    int network_socket = *((int *)void_arg);
    
    while (1)
    {
        if (recv(network_socket,server_message,sizeof(server_message),0) < 0)
        {
            printf("Error while listening to server\n");
        }

        server_message_interpreter(server_message);
    }
    
    return NULL;
}

