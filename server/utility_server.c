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
char ** extract_user_name_password(const char *client_response_smth,int n){

  int i=0;
  char *password=(char *)malloc(sizeof(char)*50);
  char *nume=(char *)malloc(sizeof(char)*50);
  memset(password,0,50);
  memset(nume,0,50);
  int dimensiuneparola=0;
  int dimensiunenume=0;
	printf("tot mesajul-----%s\n",client_response_smth);
  for(;i<n;i++)
	if(client_response_smth[i]=='-'&&i+1<n&&client_response_smth[i+1]=='u'){
		int j=i+3;
		for(;client_response_smth[j]!=' ';j++)
			nume[dimensiunenume++]=client_response_smth[j];
		i=j;

	}
        else if(client_response_smth[i]=='-'&&i+1<n&&client_response_smth[i+1]=='p'){
		int j=i+3;
		for(;j<n;j++)
			password[dimensiuneparola++]=client_response_smth[j];
                i=j;
	
	}

  char **name_password=(char **)malloc(sizeof(char *)*2);

  name_password[0]=nume;
  name_password[1]=password;  

  return name_password;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



int isInFile(char **name_password){

   FILE *fp;
   if((fp=fopen("./clienti","r"))==NULL){

	   printf("nu a putut fi deschis fisierul\n");
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
  return IS_NOT_REGISTERED;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/////////////////////////////////////////////////////////////////////////////////////////////
int login(const char *client_response_smth,int n)
{
   char **name_password=extract_user_name_password(client_response_smth,n);
   
   if(isInFile(name_password)==IS_REGISTERED) {printf("l-am gasit\n");return LOGIN_SUCCESS;
   }

   
   return LOGIN_FAIL;
   
}
/////////////////////////////////////////////////////////////////////////////////////////////








///////////////////////////////////////////////////////////////////////////////////////////////
int signup(const char *client_response_smth,int n)
{

  char **name_password=extract_user_name_password(client_response_smth,n);
  FILE * fp;
  if((fp = fopen ("./clienti", "a+"))==NULL){

	printf("nu a putut fi deschis fisierul\n");
        exit(0);
  }
  if(isInFile(name_password)==IS_REGISTERED) {printf("mai este asta incaodata\n");//return SIGNUP_FAIL;
	}
 printf("am ajns aicici %s %s \n",name_password[0],name_password[1]);

  fprintf(fp, "%s %s\n",name_password[0],name_password[1]); 
  fflush(fp);
  close(fileno(fp));

  return SIGNUP_SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////
char *pack_message(char *original_message, char flags)
{
  unsigned char message_lenght = (unsigned char)strlen(original_message);
  char message_flags = (char)flags;

  char *new_message = (char *) malloc(259);
  if(!new_message)
  {
    printf("Fail on malloc");
    exit(2);
  }

  new_message[0] = message_lenght;
  new_message[1] = message_flags;

  strcat(new_message,original_message);

    printf("Message: %d %d %s %s\n",new_message[0],new_message[1], new_message+2, new_message);

  return new_message;

}

int command(const char *client_response_smth,int client_socket)
{

  if(strstr(client_response_smth,"-d"))
  {
      printf("Closing client socket\n");
      close(client_socket);
      pthread_cancel(pthread_self());
  }

  return SUCCESS;
}
/////////////////////////////////////////////////////////////////////////////////////////////






//////////////////////////////////////////////////////////////////////////////////////////////////////
int messageInterpreter(const char client_header[2], const char *client_response_smth, int client_socket)
{
  switch(client_header[1])
  {
    case LOGIN: printf("This is a login\n");return login(client_response_smth,client_header[0]);
	break;
    case SIGNUP: printf("This is signup\n");return signup(client_response_smth,client_header[0]);break;
    case COMMAND: printf("This is a command\n"); return command(client_response_smth,client_socket);break;
    default: printf("Unhandled header parameter\n");break;
  }
  return FAIL;
}



