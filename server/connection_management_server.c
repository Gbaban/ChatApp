#include "connection_management_server.h"
#include "utility_server.h"

int client_sockets[100];
int client_sockets_dimension=0;
int nr=0;//doar verificare sa vad daca functioneaza bine
int close_connections = 0;

////////////////////////////////////////////////////////////ss/////////////////////////////////
void *handle_connection(void *vargp)
{

    nr++;
    int client_socket=*((int *)vargp);
    unsigned char client_header[2];
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
	//printf("First%s \n",client_response);
	strcpy(client_response,"\0");
	//printf("Second%s \n",client_response);
    char *client_response_smth;
    while(!close_connections)
    {
	client_response_smth = (char*)malloc(257);
          if (recv(client_socket,client_header,2,0) < 2)
        {
                printf("Error recieving data from client1\n");
                //exit(1);
        }
        else if ((recv(client_socket,client_response_smth,client_header[0],MSG_WAITALL) < client_header[0]))
        {
                printf("Error recieving data from client2\n");
                //exit(1);
        }
        else
        {
	  client_response_smth[client_header[0]] = 0;
          printf("Header %d %d\n",client_header[0],client_header[1]);
          printf("Response: %s\n", client_response_smth);

    //printf("Header compare:%d %d %d\n",client_header[1], MESSAGE, client_header[1] == MESSAGE);
    if(client_header[1] == MESSAGE)
    {
    	  int i=0;
              for(;i<client_sockets_dimension;i++)
           	  {
              		if(client_sockets[i]!=client_socket)
              		{
              			char *message1=pack_message(client_response_smth,MESSAGE);
              		  	send(client_sockets[i],message1,strlen(message1),0);
              			printf("----");
              		}
    	  }
    }
    else
    {
      printf("messageInterpreter\n");
      char *return_value = (char*)malloc(258);
      if(!return_value)
      {
        printf("Malloc error\n");
        exit(3);
      }
      sprintf(return_value,"%d",messageInterpreter(client_header,client_response_smth,client_socket));
      return_value = pack_message(return_value,client_header[1]);
      send(client_socket,return_value,strlen(return_value),0);
      free(return_value);
    }

	  free(client_response_smth);

        }

    }
    /*char *close_message = pack_message("-d",COMMAND);
    printf("Sending...\n");
    if(send(client_socket,close_message,strlen(close_message),0) <0)
    {
      printf("Error on send\n");
    }
    printf("Sent...\n");
    close(client_socket);*/

  //  free(message);



    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void close_all_connections()
{
  int i;
  char *close_message = pack_message("-d",COMMAND);
  for(i=0;i<nr;i++)
  {
    printf("Sending...\n");
    if(send(client_sockets[i],close_message,strlen(close_message),0) <0)
    {
      printf("Error on send\n");
    }
    printf("Sent...\n");
    //close(client_sockets[i]);
  }

  free(close_message);

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
