#include "connection_management_server.h"
#include "utility_server.h"

//int client_sockets[100];
//int client_sockets_dimension=0;
//int nr=0;//doar verificare sa vad daca functioneaza bine
int close_connections = 0;


////////////////////////////////////////////////////////////ss/////////////////////////////////
void *handle_connection(void *vargp)
{

    int client_socket=*((int *)vargp);
    unsigned char client_header[2];
    char client_response[256];
    if (recv(client_socket,client_header,2,0) < 0)
    {
            printf(ANSI_COLOR_RED     "[handle_connection]Error recieving data from client"     ANSI_COLOR_RESET "\n");
            //exit(1);
    }

    if (recv(client_socket,client_response,client_header[0],0) < 0)
    {
            printf(ANSI_COLOR_RED      "[handle_connection]Error recieving data from client"     ANSI_COLOR_RESET "\n");
            //exit(1);
    }


    //trimite raspunsul la toate socket-urile---de avut grija cand clientul va functiona
    //sa nu mai trimitem inapoi mesajul si acelui client care l-a trimis
    //ca va fi afisat in consola lui de 2 ori
    printf(ANSI_COLOR_BLUE    "The client sent the following data here: %s %d"    ANSI_COLOR_RESET "\n",client_response,logedin_user_dimension);
    //sprintf(server_message,"%s %d\n",server_message,nr);
    char *message = pack_message("You have reached the server!",MESSAGE,"Server");
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
                printf(ANSI_COLOR_RED     "[handle_connection]Error recieving data from client"     ANSI_COLOR_RESET "\n");
                exit(1);
        }
        else if ((recv(client_socket,client_response_smth,client_header[0],MSG_WAITALL) < client_header[0]))
        {
                printf(ANSI_COLOR_RED     "[handle_connection]Error recieving data from client"     ANSI_COLOR_RESET "\n");
                //exit(1);
        }
        else
        {
	  client_response_smth[client_header[0]] = 0;
          printf("[handle_connection]Header %d %d\n",client_header[0],client_header[1]);
          printf("[handle_connection]Response: %s\n", client_response_smth);


    //printf("Header compare:%d %d %d\n",client_header[1], MESSAGE, client_header[1] == MESSAGE);
    if(client_header[1] == MESSAGE)
    {
    	  int i=0;
              char *message1 = NULL;
              char *sender = getUsernameBySocket(client_socket);
              message1=pack_message(client_response_smth,MESSAGE,sender);
              for(;i<logedin_user_dimension;i++)
           	  {
                //printf("Users: %s\n",client_response_smth);
              		if(logedin_user_sockets[i].socket!=client_socket)
              		{
                    //printf("Message1: %s\n",message1+2);
              		  if(	send(logedin_user_sockets[i].socket,message1,strlen(message1),0) < strlen(message1)  )
                    {
                      printf(ANSI_COLOR_RED     "[handle_connection]Error on send"     ANSI_COLOR_RESET "\n");
                    }
              			printf("----");
              		}
    	        }
    }
    else
    {
      //printf("messageInterpreter\n");
      char *return_value = (char*)malloc(258);
      if(!return_value)
      {
        printf(ANSI_COLOR_RED     "Malloc error"     ANSI_COLOR_RESET "\n");
        exit(3);
      }

      int return_value_int = messageInterpreter(client_header,client_response_smth,client_socket);

      sprintf(return_value,"%d",return_value_int);
      return_value = pack_message(return_value,client_header[1],NULL);
      send(client_socket,return_value,strlen(return_value),0);
      //free(return_value);
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
  char *close_message = pack_message("-d",COMMAND,NULL);
  for(i=0;i<logedin_user_dimension;i++)
  {
    //printf("[close_all_connections]Sending...\n");
    if(send(logedin_user_sockets[i].socket,close_message,strlen(close_message),0) <0)
    {
      printf(ANSI_COLOR_RED     "[handle_connection]Error on send"     ANSI_COLOR_RESET "\n");
    }
    //printf("[close_all_connections]Sent...\n");
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
            printf(ANSI_COLOR_RED     "[manage_multiple_connections]Error accepting client socket"     ANSI_COLOR_RESET "\n");
            exit(0);
        }

        //pastrarea tuturor clientilor
      /*  client_sockets[client_sockets_dimension]=client_socket;
        client_sockets_dimension++;
        int index;
        for(index = 0; index < client_sockets_dimension;i++)
        {
          if(!client_sockets[index])
          {
              client_sockets[index] = client_sockets[client_sockets_dimension-1];
              client_sockets_dimension--;
          }
        }*/
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
