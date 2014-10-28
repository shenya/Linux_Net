#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "libnet.h"
#include "new_types.h"

#define MAX_CLIENT_NUMBER 20
#define SELECT_TIMEOUT 10

void *tcp_server_loop(void *arg)
{
    int ret = -1;
    int listen_fd = -1;
    int accept_fd = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = 0;
    int current_client_fd[MAX_CLIENT_NUMBER] = {-1};
    int client_fd_num = 0;
    int running = 1;

    fd_set read_fds;
    fd_set read_fds_temp;

    if (!arg) {
        printf("arg == null\n");
        exit(1);
    }

    memset(current_client_fd, -1, sizeof(current_client_fd));
    NET_INFO *net_info = (NET_INFO *)arg;

    listen_fd = socket_init(1, SOCK_STREAM, net_info->local_ip_addr, net_info->local_port);
    if (listen_fd < 0) {
        printf("fail to init socket\n");
        exit(1);
    }
    char buf[1024];
    int recv_len = 0;
    int max_fd = -1;
    int i = 0;
    struct timeval timeout;

    FD_ZERO(&read_fds);
    FD_ZERO(&read_fds_temp);

    FD_SET(listen_fd, &read_fds);
    max_fd = listen_fd;

    while (running) {
        read_fds_temp = read_fds;
        timeout.tv_sec = SELECT_TIMEOUT;
        timeout.tv_usec = 0;

        ret = select(max_fd + 1, &read_fds_temp, NULL, NULL, &timeout);
        if (ret < 0) {
            printf("error in select\n");
        } else if (ret == 0) {
            printf("timeout in select\n");
        } else {
            printf("fd num:%d is active\n", ret);
            if (FD_ISSET(listen_fd, &read_fds_temp)) {
                printf("fd: listen is active\n");
                addr_len = sizeof(client_addr);
                accept_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
                if (accept_fd < 0) {
                    printf("error to accept\n");
                    exit(1);
                }
                for (i = 0; i < MAX_CLIENT_NUMBER; i++) {
                    if (current_client_fd[i] == -1) {
                        current_client_fd[i] = accept_fd;
                        client_fd_num++;
                        FD_SET(accept_fd, &read_fds);
                        if (max_fd < accept_fd) {
                            max_fd = accept_fd;
                        }
                        break;
                    }
                }
            } else {
                for (i = 0; i < MAX_CLIENT_NUMBER; i++) {
                    if (FD_ISSET(current_client_fd[i], &read_fds_temp)) {
                        printf("client: %d is active to read\n", current_client_fd[i]);
                        recv_len = read(current_client_fd[i], buf, sizeof(buf) - 1);
                        if (recv_len < 0) {
                            printf("error to read\n");
                            exit(0);
                        } else if (recv_len == 0) {
                            printf("client closed\n");
                            FD_CLR(current_client_fd[i], &read_fds);
                            close(current_client_fd[i]);
                            current_client_fd[i] = -1;
                            client_fd_num--;
                        } else {
                            buf[recv_len] = '\0';
                            printf("recv buf: %s\n", buf);
                            write(current_client_fd[i], "hello", 3);
                        }
                    }
                }
            }
        }
    }

    return NULL;
}
