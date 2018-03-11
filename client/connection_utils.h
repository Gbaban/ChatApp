#ifndef CONNECTION_UTILS_H
#define CONNECTION_UTILS_H


#define FAIL 0
#define LOGIN_SUCCESS "1"
#define LOGIN_FAIL "2"
#define SIGNUP_SUCCESS "3"
#define SIGNUP_FAIL "4"
#define SUCCESS 254

int setup_socket(int port,const char *address);
void server_message_interpreter(char *server_message,unsigned char header[2]);
void* listen_to_server(void *void_arg);

#endif
