#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "libnet.h"
#include "new_types.h"

void *tcp_client_loop(void *arg)
{
    int ret = -1;
    int sock_fd = -1;
    int running = 1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = 0;
    NET_INFO *net_info = (NET_INFO *)arg;

    if (!arg) {
        printf("arg == NULL in creating tcp client\n");
        exit(1);
    }
    // TODO  need to check arg
    if (net_info->local_port < 1024) {
        sock_fd = socket_init(0, SOCK_STREAM, NULL, -1);
        if (sock_fd < 0) {
            printf("error to socket in client\n");
            exit(1);
        }
    } else {
        sock_fd = socket_init(0, SOCK_STREAM, net_info->local_ip_addr, net_info->local_port);
        if (sock_fd < 0) {
            printf("error to socket in client\n");
            exit(1);
        }
    }
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(net_info->server_port);
    server_addr.sin_addr.s_addr = inet_addr(net_info->server_ip_addr);

    char buf[1024];
    int recv_len = 0;
    int send_len = 0;
    addr_len = sizeof(server_addr);
    ret = connect(sock_fd, (struct sockaddr *)&server_addr, addr_len);
    if (ret < 0) {
        printf("error to connect\n");
    }
    printf("connect fd: %d\n", sock_fd);
    while (running) {
        printf("read something\n");
        recv_len = read(STDIN_FILENO, buf, sizeof(buf) - 1);
        if (recv_len < 0) {
            printf("error to read\n");
            exit(0);
        } else if (recv_len > 0) {
            if (strncmp(buf, "exit", 4) != 0) {
                printf("send message\n");
                send_len = write(sock_fd, buf, recv_len);
                send_len = read(sock_fd, buf, sizeof(buf));
                buf[send_len] = '\0';
                printf("recv buf: %s\n", buf);
            } else {
                running = 0;
            }
        }
    }

    printf("Client exit\n");
    close(sock_fd);
    exit(1);
    return NULL;
}
