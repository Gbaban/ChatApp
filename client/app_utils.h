#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define MESSAGE	0x80
#define LOGIN	0x40
#define SIGNUP 0x20
#define COMMAND 0x10

#define GET_PORT 1
#define GET_IP 2

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//#define DEBUG

int disconect_socket;
extern int validator;

void print_header();
char *pack_message(char *message, unsigned char flags);
void disconnect_client();
void menu();
char * get_ip_port(int argv,char *argc[],int optiune);

#endif
