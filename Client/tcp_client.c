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
#include "chat_client.h"

#define SERV_IP "127.0.0.1"
#define CLIENT_IP "127.0.0.1"
#define SERV_PORT 8888
#define CLIENT_PORT 8001

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
    int ret = -1;
    int sock_fd = -1;
    int is_default_ip_used = 1;
    int is_default_port_used = 1;
    int ch = -1;
    int server_port = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = 0;
    pthread_t client_tid = -1;
    NET_INFO net_info;

    char ip_addr[32] = "127.0.0.1";
    char server_ip_addr[32] = "127.0.0.1";

    while ((ch = getopt(argc, argv, "s:p:h")) != -1) {
        switch (ch) {
            case 's':
                is_default_ip_used = 0;
                strcpy(server_ip_addr, optarg);
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
        strcpy(net_info.server_ip_addr, SERV_IP);
    } else {
        strcpy(net_info.server_ip_addr, server_ip_addr);
    }

    if (is_default_port_used) {
        net_info.server_port = SERV_PORT;
    } else {
        net_info.server_port = server_port;
    }
    strcpy(net_info.local_ip_addr, CLIENT_IP);
    net_info.local_port = -1;
    printf("server ip is: %s\n", net_info.server_ip_addr);
    printf("server port is: %d\n", net_info.server_port);
    if (pthread_create(&client_tid, NULL, tcp_client_loop,
            (void *)&net_info) < 0) {
        printf("create pthread client failed\n");
        exit(1);
    }

    while(1);
    return 0;
}
