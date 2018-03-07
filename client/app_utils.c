#include "app_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

void print_header()
{
    printf("+**********************************************************************+\n");
    printf("*                                 CHAT APP                             *\n");
    printf("*                               CLIENT VERSION                         *\n");
    printf("+**********************************************************************+\n\n\n");
}


char *pack_message(char *message, char flags ){

	char message_length;
	message_length = strlen(message);

	char *final_message = (char *) malloc(258);
	final_message[0] = message_length+1;
    	final_message[1] = flags;

	strcat(final_message,message);
    printf("final: %d %d %s\n",final_message[0],final_message[1],final_message+2);

	return final_message;
}

void message_loop(int socket)
{
    char message[256];
    while (1)
    {
        fgets(message,sizeof(message),stdin);
		message[strlen(message) - 1] = '\0';
	if(strlen(message) == 0)
	{
		printf("No blank message\n");
	} else
  {
        if (strchr(message,29))
        {
          printf("ctrl+]\n");
          //TODO sendCommand() implementation to scan input and send commands to server with COMMAND header
          //sendCommand();
        }
        else if (strlen(message) > 256)
        {
            printf("Messages should be lower than 256 characters\n");
        }
        else
        {
		         strcpy(message, pack_message(message, MESSAGE));
             send(socket, message, strlen(message), 0);
        }
    }
  }
}

void signup(int socket)
{
    char username[30];
    char password[30];
    char repeated_password[30];

    do
    {
        printf("Username: ");
        scanf("%99s",username);
        if (strlen(username) >= 30)
        {
            printf("Username should be lower than 30 characters\n");
        }
    } while (strlen(username) >= 30);

    do
    {
        printf("Password: ");
        scanf("%99s",password);
        if (strlen(username) >= 30)
        {
            printf("Password should be lower than 30 characters\n");
        }
    } while (strlen(password) >= 30);

    do
    {
        printf("Repeat password: ");
        scanf("%99s",repeated_password);
        if (strlen(username) >= 30)
        {
            printf("Password should be lower than 30 characters\n");
        }
    } while (strlen(repeated_password) >= 30);

    if (!strcmp(password,repeated_password))
    {
		char signup_message[256] = "";
		strcat(signup_message, "-u ");
		strcat(signup_message, username);
		strcat(signup_message, " -p ");
		strcat(signup_message, password);
		strcpy(signup_message, pack_message(signup_message, SIGNUP));
    //printf("Sending.......\n");
        send(socket, signup_message, strlen(signup_message), 0);
    //printf("Sent\n");
    }
    else
    {
        printf("The repeated password must be identical to the original!\n");
    }

	fflush(stdin);
    message_loop(socket);
}

void login(int socket)
{
    char username[30];
    char password[30];

    do
    {
        printf("Username: ");
        scanf("%99s",username);
        if (strlen(username) >= 30)
        {
            printf("Username should be lower than 30 characters\n");
        }
    } while (strlen(username) >= 30);

    do
    {
        printf("Password: ");
        scanf("%99s",password);
        if (strlen(username) >= 30)
        {
            printf("Password should be lower than 30 characters\n");
        }
    } while (strlen(password) >= 30);


	char login_message[256] = "";
	strcat(login_message, "-u ");
	strcat(login_message, username);
	strcat(login_message, " -p ");
	strcat(login_message, password);
	strcpy(login_message, pack_message(login_message, LOGIN));

    send(socket, login_message, strlen(login_message), 0);

	//TODO check if logged in before entering chat room
    message_loop(socket);
}

void disconnect_client(int sig)
{
    char message[] = "-d";

    if(send(disconect_socket,pack_message(message,COMMAND),strlen(pack_message(message,COMMAND)),0) < 0)
    {
      printf("Error on send\n");
    }

    close(disconect_socket);

    printf("You have disconnected\n");

    exit(0);
}

void menu(pthread_t tid, int socket)
{
    int input;
    do
    {
        printf("1 - Sign Up\n");
        printf("2 - Login\n");
        printf("0 - Exit\n\n");
        printf("Enter input: ");
        scanf("%d", &input);

        switch (input)
        {
            case 1: { signup(socket); input = 0; break; }
            case 2: { login(socket); input = 0; break; }
            case 0: { disconnect_client(socket); break; }
            default: { printf("\nERROR: Invalid input!\n"); }
        }
    } while (input != 0);
}


char * get_ip_port(int argv,char *argc[],int optiune)
{

	int i;

    for( i=1;i<argv;i++)
        if(optiune==GET_PORT&&i<argv&&strcmp(argc[i],"-p")==0)
        {
            return argc[i+1];
        }
        else if(optiune==GET_IP&&i<argv&&strcmp(argc[i],"-ip")==0)
        {
            return argc[i+1];
        }
    return NULL;

}
