#include "connection_utils.h"
#include "app_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>





int setup_socket(int port,const char *address)
{

    //printf("Setup_socket\n");
    int network_socket;
    network_socket = socket(AF_INET,SOCK_STREAM,0);
    if (!network_socket)
    {
        printf(ANSI_COLOR_RED     "[setup_socket]Error creating socket in client"     ANSI_COLOR_RESET "\n");
        exit(0);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(address); //use when we actually get an IP
    //server_address.sin_addr.s_addr = INADDR_ANY; //use for debug purposes

    if (connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf(ANSI_COLOR_RED     "[setup_socket]Error establishing connection in client socket"     ANSI_COLOR_RESET "\n");
        exit(0);
    }

    //handshaking
    char message[] = "Sup, I connected!\n";
    if (send(network_socket,pack_message(message,1),strlen(pack_message(message,1)),0) < 0)
    {
    	printf("[setup_socket]Error sending message\n");
    }

    char server_response[256];
    unsigned char header[2];
    if (recv(network_socket,header,2,MSG_WAITALL) < 0)
    {
        printf(ANSI_COLOR_RED     "[setup_socket]Error while listening to server"     ANSI_COLOR_RESET "\n");
        close(network_socket);
    }
    //MSG_WAITALL
    else if(recv(network_socket,server_response,header[0],MSG_WAITALL) < 0)
    {
      printf(ANSI_COLOR_RED     "[setup_socket]Error while listening to server"     ANSI_COLOR_RESET "\n");
      close(network_socket);
    }
    else{
      //printf("Message incoming: %d %d %s", header[0], header[1], server_response);
    server_message_interpreter(server_response,header);
  }

    return network_socket;
}

void server_message_interpreter(char *server_message, unsigned char header[2])
{
    //printf("Recieved from server: %s\n",server_message);

    switch(header[1])
    {
        case MESSAGE:
            {
              #ifdef DEBUG
                printf("[server_message_interpreter]mesaj: %s\n",server_message);
              #endif
              printf("%s\n",server_message);
              break;
            } //mesaj
        case LOGIN:
            {
                #ifdef DEBUG
                  printf("[server_message_interpreter]login\n");
                #endif
                if (strstr(server_message,LOGIN_SUCCESS)) //LOGIN_SUCCESS
		        {
			        validator = 1;
              printf(ANSI_COLOR_GREEN     "Login was succesfull"     ANSI_COLOR_RESET "\n");
		        }
		        else if (strstr(server_message,LOGIN_FAIL)) //LOGIN_FAIL
		        {
			        validator = 2;
		        }
		        break;
          } //logins
        case SIGNUP:
            {
                #ifdef DEBUG
                  printf("[server_message_interpreter]signup\n");
                #endif
                if (strstr(server_message,SIGNUP_SUCCESS)) //SIGNUP_SUCCESS
		        {
			        validator = 1;
              printf(ANSI_COLOR_GREEN     "Signup was succesfull"     ANSI_COLOR_RESET "\n");
		        }
		        else if (strstr(server_message,SIGNUP_FAIL)) //SIGNUP_FAIL
		        {
			        validator = 3;
		        }
		        break;
            } //signup
        case COMMAND:
        {
          #ifdef DEBUG
            printf("[server_message_interpreter]comanda : %s\n", server_message);
          #endif
          if(strstr(server_message,"-d"))
          {
              #ifdef DEBUG
                printf("[server_message_interpreter]Closing socket\n");
              #endif
              printf("Closing socket\n");
              disconnect_client(0);
          }

          break;
        } //comanda
    }

	   //free(server_message);

}

void* listen_to_server(void *void_arg)
{
    char server_message[256];
    unsigned char header[2];

    int network_socket = *((int *)void_arg);

    while (1)
    {
        if (recv(network_socket,header,2,0) < 2)
        {
            printf(ANSI_COLOR_RED     "[listen_to_server]Error while listening to server"     ANSI_COLOR_RESET "\n");
            exit(1);
        }
        else if(recv(network_socket,server_message,header[0],0) < header[0])
        {
          printf(ANSI_COLOR_RED     "[listen_to_server]Error while listening to server"     ANSI_COLOR_RESET "\n");
        }
        else{
	         server_message[header[0]] = 0;
           #ifdef DEBUG
            printf("[listen_to_server]Header %d %d\n",header[0],header[1]);
            printf("[listen_to_server]Response: %s\n", server_message);
           #endif
        server_message_interpreter(server_message,header);
        strcpy(server_message,"");
      }
    }

    return NULL;
}
