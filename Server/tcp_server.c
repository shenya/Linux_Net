#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "libnet.h"
#include "new_types.h"
#include "chat_server.h"

#define SERV_IP "127.0.0.1"
#define SERV_PORT 8888

void help()
{
    printf("--------------------help------------------\n");
    printf("    usage: ./serv -s x.x.x.x -p port\n");
    printf("    -s: server's IP\n");
    printf("    -p: server's port\n");
    printf("    -h: help\n");
}

int main(int argc, char **argv)
{
    int ch = -1;
    int is_default_ip_used = 1;
    int is_default_port_used = 1;

    pthread_t server_tid = -1;
    NET_INFO net_info;
    char ip_addr[32] = "127.0.0.1";
    int server_port = -1;

    while ((ch = getopt(argc, argv, "s:p:h")) != -1) {
        switch (ch) {
            case 's':
                is_default_ip_used = 0;
                strcpy(ip_addr, optarg);
                break;
            case 'p':
                server_port = atoi(optarg);
                if (server_port > 1024 && server_port < 65535) {
                    is_default_port_used = 0;
                }
                break;
            case 'h':
            default:
                help();
                exit(0);
                break;
        }
    }

    if (is_default_ip_used) {
        strcpy(net_info.local_ip_addr, SERV_IP);
    } else {
        strcpy(net_info.local_ip_addr, ip_addr);
    }

    if (is_default_port_used) {
        net_info.local_port = SERV_PORT;
    } else {
        net_info.local_port = server_port;
    }
    printf("local ip is: %s\n", net_info.local_ip_addr);
    printf("local port is: %d\n", net_info.local_port);
    printf("start server with local ip and local port\n");

    if (pthread_create(&server_tid, NULL, tcp_server_loop,
            (void *)&net_info) < 0) {
        printf("create pthread failed\n");
        exit(1);
    }

    while(1);
    return 0;
}
