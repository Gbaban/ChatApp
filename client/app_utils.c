#include "app_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void print_header()
{
    printf("+**********************************************************************+\n");
    printf("*                                 CHAT APP                             *\n");
    printf("*                               CLIENT VERSION                         *\n");
    printf("+**********************************************************************+\n\n\n");
}

void signup()
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
        //TODO send username and password to server to be added to database
    }
    else
    {
        printf("The repeated password must be identical to the original!\n");
    }
}

void login()
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
    
    //TODO send username and password to server for authentication
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
