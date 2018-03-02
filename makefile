hellomake: client.c server_stub.c
	gcc -o client client.c -lpthread
	gcc -o server_stub server_stub.c -lpthread
