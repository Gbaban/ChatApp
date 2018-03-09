#include "utility_server.h"


#define GET_PORT 1
#define GET_IP 2

///////////////////////////////////////////////////////////////////////////////////////////////////////////
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char ** extract_user_name_password(const char *client_response_smth,int m){

  int i=0;
  char *password=(char *)malloc(sizeof(char)*50);
  char *nume=(char *)malloc(sizeof(char)*50);
  memset(password,0,50);
  memset(nume,0,50);
  int dimensiuneparola=0;
  int dimensiunenume=0;
  int n=strlen(client_response_smth);

	printf("[extract_user_name_password]tot mesajul-----%s\n",client_response_smth);
  for(;i<n;i++)
	if(client_response_smth[i]=='-'&&i+1<n&&client_response_smth[i+1]=='u'){
		int j=i+3;
		for(;client_response_smth[j]!=' '&&j<n;j++)
			nume[dimensiunenume++]=client_response_smth[j];
		i=j;

	}
        else if(client_response_smth[i]=='-'&&i+1<n&&client_response_smth[i+1]=='p'){
		int j=i+3;
		for(;client_response_smth[j]!='\0'&&j<n;j++)
			password[dimensiuneparola++]=client_response_smth[j];
                i=j;

	}

  char **name_password=(char **)malloc(sizeof(char *)*2);
  
  name_password[0]=nume;
  name_password[1]=password;

  printf("ajung pana aicii\n");

  return name_password;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int isInFile(char **name_password){
   /*
   FILE *fp;
   if((fp=fopen("./totallyLegitSecureUserDB.user","r"))==NULL){

	   printf(ANSI_COLOR_RED     "[signup]File could not be opened"     ANSI_COLOR_RESET "\n");
           exit(1);
   }
   char nume[50];
   char password[50];
   int nr=0;
   while((nr=fscanf(fp,"%s %s",nume,password))==2)
   {
	if(strcmp(nume,name_password[0])==0&&strcmp(password,name_password[1])==0){
	   fclose(fp);
	   return IS_REGISTERED;
	}
   }
  close(fileno(fp));
*/

  FILE *fpbinar;
  if((fpbinar=fopen("./totallyLegitSecureUserDb.user.bin","rb"))==NULL){

	printf(ANSI_COLOR_RED     "[signup]File could not be opened"     ANSI_COLOR_RESET "\n");
           exit(1);
        
  }
  
  int nr=0;
  char nume[50];
  while((nr=fread(nume,40,1,fpbinar))!=0){

	char password[50];
        fread(password,40,1,fpbinar);
        printf("afisare ---- %s %s\n",nume,password);
        if(strcmp(nume,name_password[0])==0&&strcmp(password,name_password[1])==0){

	   close(fileno(fpbinar));
           return IS_REGISTERED;
	}

  }
  close(fileno(fpbinar));


  return IS_NOT_REGISTERED;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////
int login(const char *client_response_smth,int n)
{
   char **name_password=extract_user_name_password(client_response_smth,n);

   if(isInFile(name_password)==IS_REGISTERED)
	 {
		 	printf(ANSI_COLOR_GREEN   "[login]User found"   ANSI_COLOR_RESET "\n");return LOGIN_SUCCESS;
   }
	 printf(ANSI_COLOR_RED     "[login]User NOT found"     ANSI_COLOR_RESET "\n");
   return LOGIN_FAIL;

}
/////////////////////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////////////////////
int signup(const char *client_response_smth,int n)
{

  char **name_password=extract_user_name_password(client_response_smth,n);
  FILE * fp;
  if((fp = fopen ("./totallyLegitSecureUserDB.user", "a+"))==NULL){

	printf(ANSI_COLOR_RED     "[signup]File could not be opened"     ANSI_COLOR_RESET "\n");
        exit(0);
  }

  FILE * fpbinar;
  if((fpbinar=fopen("./totallyLegitSecureUserDb.user.bin","ab"))==NULL){

	printf(ANSI_COLOR_RED     "[signup]File binary could not be opened"     ANSI_COLOR_RESET "\n");
        exit(0);

  }

  if(isInFile(name_password)==IS_REGISTERED)
	{
		printf(ANSI_COLOR_YELLOW  "[signup]User already in DB"  ANSI_COLOR_RESET "\n");
		return SIGNUP_FAIL;
	}
 //printf("[signup]am ajns aicici %s %s \n",name_password[0],name_password[1]);

  fprintf(fp, "%s %s\n",name_password[0],name_password[1]);
  fwrite(name_password[0],40,1,fpbinar);
  fwrite(name_password[1],40,1,fpbinar);
  fflush(fp);
  close(fileno(fp));
  fflush(fpbinar);
  close(fileno(fpbinar));

  return SIGNUP_SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////


char *getUsernameBySocket(int client_socket)
{
	char *username =  (char *)malloc(31);
	if(!username)
	{
		printf(ANSI_COLOR_RED     "[getUsernameBySocket]Fail on malloc"     ANSI_COLOR_RESET "\n");
		exit(5);
	}

	int i;
	for(;i<logedin_user_dimension;i++)
	{
		//printf("Users: %s\n",client_response_smth);
			if(logedin_user_sockets[i].socket==client_socket)
			{
				strcpy(username,logedin_user_sockets[i].name);
				return username;
			}
	}

	return NULL;
}




//////////////////////////////////////////////////////////////////////////////////////////////
char *pack_message(char *original_message, unsigned char flags, const char *sender)
{
  unsigned char message_lenght = (unsigned char)strlen(original_message);

  char *new_message = NULL;
	new_message =  (char *)malloc(259);
  if(!new_message)
  {
    printf(ANSI_COLOR_RED     "[pack_message]Fail on malloc"     ANSI_COLOR_RESET "\n");
    exit(2);
  }

	//printf("NewMessage: %s\n",new_message);

  new_message[0] = message_lenght;
  new_message[1] = (char)flags;

	if(sender)
	{
		strcat(new_message,sender);
		strcat(new_message,": ");
	}
  strcat(new_message,original_message);
	new_message[0] = (unsigned char)strlen(new_message)-2;

    printf("[pack_message]Message: %d %d %s %s\n",new_message[0],new_message[1], new_message+2, original_message);

  return new_message;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

char *listAll(int client_socket)
{
	char *activeUsers =  (char *)malloc(257);
  if(!activeUsers)
  {
    printf(ANSI_COLOR_RED     "[listAll]Fail on malloc"     ANSI_COLOR_RESET "\n");
    exit(4);
  }
	int i;
	for(;i<logedin_user_dimension;i++)
	{
		//printf("Users: %s\n",client_response_smth);
			if(logedin_user_sockets[i].socket!=client_socket)
			{
				strcat(activeUsers,", ");
				strcat(activeUsers,logedin_user_sockets[i].name);
			}
	}

	return activeUsers;


}

/////////////////////////////////////////////////////////////////////////////////////////////////////

int command(const char *client_response_smth,int client_socket)
{

  if(strstr(client_response_smth,"-d"))
  {
      printf("Closing client socket\n");
      close(client_socket);
			int i=0;
			for(;i<logedin_user_dimension;i++)
			{
				if(logedin_user_sockets[i].socket == client_socket)
				{
					break;
				}
			}

			printf("[command]client_socket: %d   %d\n",logedin_user_sockets[i].socket, client_socket);
                        if(logedin_user_dimension-1>=0)
			{ 
                         logedin_user_sockets[i] = logedin_user_sockets[logedin_user_dimension-1];
			 logedin_user_dimension--;
                        }
			pthread_cancel(pthread_self());
  }
	else if(strstr(client_response_smth,"listall"))
	{

		printf("ListAll\n");

		char *message = pack_message(listAll(client_socket),MESSAGE,"Server");
    send(client_socket,message,strlen(message),0);
    free(message);


	}

  return SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////
int messageInterpreter(const char client_header[2], const char *client_response_smth, int client_socket)
{
  switch(client_header[1])
  {
    case LOGIN:
		{
			printf("[messageInterpreter]This is a login\n");
			int return_value =  login(client_response_smth,client_header[0]);
			if (return_value == LOGIN_SUCCESS)
			{
				logedin_user_sockets[logedin_user_dimension].socket = client_socket;
				char **name_password=extract_user_name_password(client_response_smth,client_header[0]);
				strcpy(logedin_user_sockets[logedin_user_dimension].name,name_password[0]);
				logedin_user_dimension++;
                                if(logedin_user_dimension-1>=0)
				printf("User <" ANSI_COLOR_BLUE     "%s" ANSI_COLOR_RESET "> loged in\n",logedin_user_sockets[logedin_user_dimension-1].name); 
			}
			return return_value;
		}break;
    case SIGNUP:
		{
			printf("[messageInterpreter]This is signup\n");
			int return_value_signup =  signup(client_response_smth,client_header[0]);
			if (return_value_signup == SIGNUP_SUCCESS)
			{
				logedin_user_sockets[logedin_user_dimension].socket = client_socket;
				char **name_password=extract_user_name_password(client_response_smth,client_header[0]);
				strcpy(logedin_user_sockets[logedin_user_dimension].name,name_password[0]);
				logedin_user_dimension++;
                                if(logedin_user_dimension-1>=0)
				printf("User <" ANSI_COLOR_BLUE     "%s" ANSI_COLOR_RESET "> loged in\n",logedin_user_sockets[logedin_user_dimension-1].name);
			}
			return return_value_signup;
		}break;
    case COMMAND: printf("[messageInterpreter]This is a command\n"); return command(client_response_smth,client_socket);break;
    default: printf("[messageInterpreter]Unhandled header parameter\n");break;
  }
  return FAIL;
}
