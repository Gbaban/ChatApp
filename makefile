hellomake: client/client.c server/server_stub.c
	gcc -o client/client client/client.c -lpthread
	gcc -o server/server_stub server/server_stub.c server/utility_server.c server/connection_management_server.c -lpthread
