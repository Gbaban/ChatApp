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


char *pack_message(char *message, int bit){

	char message_length[1];
	message_length[0] = strlen(message);

	char header_data[1];
	header_data[0] = 0b00000000;
	if(bit == 0){
		header_data[0] = 0b00000000;	//comanda
	}
	if(bit == 1){
		header_data[0] = 0b10000000;	//mesaj
	}
	
	if(bit == 2){
		header_data[0] = 0b01000000;	//login
	}

	if(bit == 3){
		header_data[0] = 0b00100000;	//signup
	}

	char *final_message = (char *) malloc(258);
	strcat(final_message, message_length);
	strcat(final_message, header_data);
	strcat(final_message, message);

	return final_message;
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
		strcpy(signup_message, pack_message(signup_message, 3));

        send(socket, signup_message, sizeof(signup_message), 0);
    }
    else
    {
        printf("The repeated password must be identical to the original!\n");
    }
}


void message_loop(int socket)
{
    char message[256];
    while (1)
    {
        fgets(message,sizeof(message),stdin);
		message[strlen(message) - 1] = '\0';
        if (strlen(message) > 256)
        {
            printf("Messages should be lower than 256 characters\n");
        }
        else
        {
			strcpy(message, pack_message(message, 1));
            send(socket, message, sizeof(message), 0);
        }
    }
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
	strcpy(login_message, pack_message(login_message, 2));
    
	//TODO check if logged in before entering chat room
    message_loop(socket);
}

void menu(int socket)
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
            case 1: { signup(socket); break; }
            case 2: { login(socket); input = 0; break; }
            case 0: { exit(0); break; }
            default: { printf("\nERROR: Invalid input!\n"); }
        }
    } while (input != 0);
}
