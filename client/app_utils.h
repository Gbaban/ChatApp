#ifndef APP_UTILS_H
#define APP_UTILS_H

#define MESSAGE	0x80
#define LOGIN	0x40
#define SIGNUP 0x20
#define COMMAND 0x10

#define GET_PORT 1
#define GET_IP 2

int disconect_socket;

void print_header();
char *pack_message(char *message, char flags);
void disconnect_client();
void menu();
char * get_ip_port(int argv,char *argc[],int optiune);

#endif
