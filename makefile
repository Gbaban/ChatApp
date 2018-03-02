server: server/server_stub.c server/utility_server.c server/connection_management_server.c client.o
	gcc -o server/server_stub server/server_stub.c server/utility_server.c server/connection_management_server.c -lpthread

client.o: client/client.c
	gcc -o client/client client/client.c -lpthread
