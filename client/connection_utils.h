#ifndef CONNECTION_UTILS_H
#define CONNECTION_UTILS_H

int setup_socket(int port);
void server_message_interpreter(char *server_message, char header[2]);
void* listen_to_server(void *void_arg);

#endif
