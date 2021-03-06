#include "app_utils.h"
#include <ctype.h>
#include <string.h>

int validator;

void print_header()
{
    printf("+**********************************************************************+\n");
    printf("*                                 CHAT APP                             *\n");
    printf("*                               CLIENT VERSION                         *\n");
    printf("+**********************************************************************+\n\n\n");
}


char *pack_message(char *message,unsigned char flags ){

	char message_length;
	message_length = strlen(message);

  #ifdef DEBUG
    	printf("initial:%s\n",message);
  #endif

	char *final_message = (char *) malloc(258);
	final_message[0] = message_length;
  final_message[1] = flags;
  final_message[2] = 0;

	strcat(final_message,message);
  #ifdef DEBUG
    	printf("final: %d  %d  %s\n\n\n",final_message[0],flags,final_message);
  #endif

	return final_message;
}

void sendCommand()
{
    char command[256];

    scanf("%255s",command);

    if (!strcmp(command,"listall"))
    {
        if(send(disconect_socket,pack_message(command,COMMAND),strlen(pack_message(command,COMMAND)),0) < 0)
        {
            printf(ANSI_COLOR_RED     "[sendCommand]Error on send"     ANSI_COLOR_RESET "\n");
        }
    }
    else
    {
        printf(ANSI_COLOR_RED     "[sendCommand]Invalid command"     ANSI_COLOR_RESET "\n");
    }

    //can be extended to further commands
}

void message_loop(int socket)
{
    while (1)
    {
      // #ifdef DEBUG
        // printf("[message_loop]validator: %d",validator);
      // #endif

        switch (validator)
        {
            case 1:
            {
                char message[256];
                while (1)
                {
                    fgets(message,sizeof(message),stdin);
                    message[strlen(message) - 1] = '\0';
                if(strlen(message) == 0)
                {
                    #ifdef DEBUG
                      printf("No blank message\n");
                    #endif
                } else
              {
                    if (strchr(message,29))
                    {
                      #ifdef DEBUG
                        printf("[message_loop]ctrl+]\n");
                      #endif
                        printf(ANSI_COLOR_CYAN "Command\n");
                      sendCommand();
                        printf(ANSI_COLOR_RESET);
                    }
                    else if (strlen(message) > 256)
                    {
                        printf(ANSI_COLOR_YELLOW     "Messages should be less than 256 characters"     ANSI_COLOR_RESET "\n");
                    }
                    else
                    {
                             strcpy(message, pack_message(message, MESSAGE));
                         if (send(socket, message, strlen(message), 0) < 0)
                         {
                         	printf("[message_loop]Error sending message\n");
                         }
                    }
                }
              }
              break;
           }

           case 2:
           {
                printf(ANSI_COLOR_RED     "Invalid credentials or username not found. Please try again"     ANSI_COLOR_RESET "\n");
		            validator = 0;
                return;
           }

           case 3:
           {
               printf(ANSI_COLOR_RED     "Username is already taken. Try another"     ANSI_COLOR_RESET "\n");
 	             validator = 0;
               return;
           }

        }
  }
}

void signup(int socket)
{
    char username[31];
    char *password;
    char password_aux[31];


    do
    {
        printf("Username: ");
        scanf("%30s",username);
        if (strlen(username) >= 30)
        {
            printf(ANSI_COLOR_YELLOW     "Username should be less than 30 characters"     ANSI_COLOR_RESET "\n");
        }
    } while (strlen(username) >= 30);

    do
    {
        //printf("Password: ");
        //scanf("%99s",password);
        password = getpass("Password: ");
        #ifdef DEBUG
          printf("Password: %s\n", password);
        #endif
        if (strlen(password) >= 30)
        {
            printf(ANSI_COLOR_YELLOW     "Password should be less than 30 characters"     ANSI_COLOR_RESET "\n");
        }
    } while (strlen(password) >= 30);

    strcpy(password_aux,password);

    do
    {
        //printf("Repeat password: ");
        //scanf("%99s",repeated_password);
        password = getpass("Repeat password: ");
        #ifdef DEBUG
          printf("Repeated_password: %s\n", password);
        #endif
        if (strlen(password) >= 30)
        {
            printf(ANSI_COLOR_YELLOW     "Password should be less than 30 characters"     ANSI_COLOR_RESET "\n");
        }
    } while (strlen(password) >= 30);

    if (strcmp(password_aux,password) == 0)
    {
		char signup_message[256] = "";
		strcat(signup_message, "-u ");
		strcat(signup_message, username);
		strcat(signup_message, " -p ");
		strcat(signup_message, password);
		strcpy(signup_message, pack_message(signup_message, SIGNUP));

        if (send(socket, signup_message, strlen(signup_message), 0) < 0)
        {
             	printf("[signup]Error sending message\n");
        }
    //printf("Sent\n");
    }
    else
    {
        printf(ANSI_COLOR_YELLOW     "The repeated password must be identical to the original!"     ANSI_COLOR_RESET "\n");
        return;
    }

	fflush(stdin);
    message_loop(socket);
}

void login(int socket)
{
    char username[31];
    char *password;

    do
    {
        printf("Username: ");
        scanf("%30s",username);
        if (strlen(username) >= 30)
        {
            printf(ANSI_COLOR_YELLOW     "Username should be less than 30 characters"     ANSI_COLOR_RESET "\n");
        }
    } while (strlen(username) >= 30);

    do
    {
        password = getpass("Password: ");
        #ifdef DEBUG
          printf("Password: %s\n", password);
        #endif

        if (strlen(password) >= 30)
        {
            printf(ANSI_COLOR_YELLOW     "Password should be less than 30 characters"     ANSI_COLOR_RESET "\n");
        }
    } while (strlen(password) >= 30);


	char login_message[256] = "";
	strcat(login_message, "-u ");
	strcat(login_message, username);
	strcat(login_message, " -p ");
	strcat(login_message, password);
	strcpy(login_message, pack_message(login_message, LOGIN));
    if(send(socket, login_message, strlen(login_message), 0) < 0)
    {
    	printf("[login]Error sending message\n");
    }

    message_loop(socket);
}

void disconnect_client(int sig)
{
    char message[] = "-d";

    if(send(disconect_socket,pack_message(message,COMMAND),strlen(pack_message(message,COMMAND)),0) < 0)
    {
      printf(ANSI_COLOR_RED     "[disconnect_client]Error on send"     ANSI_COLOR_RESET "\n");
    }

    close(disconect_socket);

    printf(ANSI_COLOR_BLUE    "You have disconnected"    ANSI_COLOR_RESET "\n");

    exit(0);
}

void menu(pthread_t tid, int socket)
{
    char input[10];
    do
    {
        printf("1 - Sign Up\n");
        printf("2 - Login\n");
        printf("0 - Exit\n\n");
        printf("Enter input: ");
        scanf("%s", input);
		if(strlen(input) == 1 && isdigit(input[0]) && atoi(input) >= 0 && atoi(input) <= 2){
			
		    switch (input[0])
		    {
		        case '1': { signup(socket);break; }
		        case '2': { login(socket);break; }
		        case '0': { disconnect_client(socket); break; }
		        default: { printf("\n" ANSI_COLOR_RED     "ERROR: Invalid input!"     ANSI_COLOR_RESET "\n"); }
		    }
		}
		else{
			printf("\n"	ANSI_COLOR_RED		"Not a valid input"		ANSI_COLOR_RESET	"\n");
		}
    } while (strcmp(input, "0") != 0);
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
