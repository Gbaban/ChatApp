#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDRESS "192.168.0.102"
#define PORT 9002

int network_socket;

void setup_socket()
{
    network_socket = socket(AF_INET,SOCK_STREAM,0);
    if (!network_socket)
    {
        printf("Error creating socket in client\n");
        exit(0);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(ADDRESS); //use when we actually get an IP
    server_address.sin_addr.s_addr = INADDR_ANY; //use for debug purposes

    if (connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address)) < 0)
    {
        printf("Error establishing connection in client socket\n");
        exit(0);
    }

    //beta handshaking
    char message[] = "Sup, I connected!\n";
    send(network_socket,message,sizeof(message),0);

    // oti: vezi ca aici eu am pus bucla de while(1)....sterge-o daca te deranjeaza doar ca am vrut sa o testez la mine

    while(1){
    char server_response[256];
    if (recv(network_socket,server_response,sizeof(server_response),0) < 0)
    {
        printf("Error recieving data from server\n");
        exit(1);
    }

    //print out the server's response
    printf("The server sent the data: %s\n",server_response);
    }
}

void print_header()
{
    printf("+**********************************************************************+\n");
    printf("*                                 CHAT APP                             *\n");
    printf("*                               CLIENT VERSION                         *\n");
    printf("+**********************************************************************+\n\n\n");
}

void signup()
{
    printf("In signup function\n");
}

void login()
{
    printf("In login function\n");
}

void menu()
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
            case 1: { signup(); break; }
            case 2: { login(); input = 0; break; }
            case 0: { break; }
            default: { printf("\nERROR: Invalid input!\n"); }
        }
    } while (input != 0);
}

int main()
{
    setup_socket();
    print_header();
    menu();
    close(network_socket);
    return 0;
}
