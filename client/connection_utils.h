#ifndef CONNECTION_UTILS_H
#define CONNECTION_UTILS_H

int setup_socket();
void server_message_interpreter(char *server_message);
void* listen_to_server(void *void_arg);

#endif