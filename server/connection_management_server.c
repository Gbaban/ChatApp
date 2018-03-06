#include "connection_management_server.h"


int client_sockets[100];
int client_sockets_dimension=0;
int nr=0;//doar verificare sa vad daca functioneaza bine
int close_connections = 0;



///////////////////////////////////////////////////////////////////////////////////////////////////

char *pack_message(char *original_message, char flags)
{
  unsigned char message_lenght = (unsigned char)strlen(original_message);
  char message_flags = (char)flags;

  char *message = (char *) malloc(259);

  message[0] = message_lenght;
  message[1] = message_flags;

  strcat(message,original_message);

    printf("Message: %d %d %s %s\n",message[0],message[1], message+2, message);

  return message;

}
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
void *handle_connection(void *vargp)
{

    nr++;
    int client_socket=*((int *)vargp);
    signed char client_header[2];
    char client_response[256];
    if (recv(client_socket,client_header,2,0) < 0)
    {
            printf("Error recieving data from client\n");
            //exit(1);
    }

    if (recv(client_socket,client_response,client_header[0],0) < 0)
    {
            printf("Error recieving data from client\n");
            //exit(1);
    }


    //trimite raspunsul la toate socket-urile---de avut grija cand clientul va functiona
    //sa nu mai trimitem inapoi mesajul si acelui client care l-a trimis
    //ca va fi afisat in consola lui de 2 ori
    printf("The client sent the following data here: %s %d\n",client_response,nr);
    //sprintf(server_message,"%s %d\n",server_message,nr);
    char *message = pack_message("You have reached the server!",MESSAGE);
    //printf("Sending message\n");
    send(client_socket,message,strlen(message),0);
    //printf("Message sent\n");
    free(message);
    /*int i=0;
    for(;i<client_sockets_dimension;i++)*/
            //send(client_sockets[i],server_message,sizeof(server_message),0);
        ////////////////////////////////////////////////////////////////////////////

    //loop until connection needs to be closed
	printf("First%s \n",client_response);
	strcpy(client_response,"\0");
	printf("Second%s \n",client_response);
    char *client_response_smth;
    while(!close_connections)
    {
	client_response_smth = (char*)malloc(257);
          if (recv(client_socket,client_header,2,0) < 0)
        {
                printf("Error recieving data from client1\n");
                //exit(1);
        }
        else if ((recv(client_socket,client_response_smth,client_header[0],0) <= 0))
        {
                printf("Error recieving data from client\n");
                //exit(1);
        }
        else
        {
	  client_response_smth[client_header[0]-1] = 0;
          printf("Header %d %d\n",client_header[0],client_header[1]);
          printf("Response: %s\n", client_response_smth);
	  /*int i=0;
          for(;i<client_sockets_dimension;i++){
	  	send(client_sockets[i],client_response_smth,strlen(client_response_smth),0);
		printf("----");
	  }
	*/
	  free(client_response_smth);
	  
        }

    }
    message = pack_message("-d\0",COMMAND);
    printf("Sending...\n");
    send(client_socket,message,strlen(message),0);
    printf("Sent...\n");
    close(client_socket);

    free(message);



    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void close_all_connections()
{
  close_connections = 1;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void manage_multiple_connections(int server_socket)
{



    while(1)
    {
        int client_socket;

        client_socket = accept(server_socket,NULL,NULL);

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
        //pthread_join(tid, NULL);
        //////////////////////////////////////////////////////////////////

    }//loop for incoming connections
    close(server_socket);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
