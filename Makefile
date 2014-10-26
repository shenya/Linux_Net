all:
	gcc tcp_server.c -o serv
	gcc tcp_client.c -o cli
