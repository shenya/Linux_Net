#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


#define SERV_IP "127.0.0.1"
#define SERV_PORT 8888

int main(int argc, char **argv)
{
    int ret = -1;
    int listen_fd = -1;
    int accept_fd = -1;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t addr_len = 0;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        printf("error to listen_fd\n");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERV_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERV_IP);

    ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        printf("error to bind\n");
    }

    ret = listen(listen_fd, 20);
    if (ret < 0) {
        printf("error to listen\n");
        return -1;
    }

    char buf[1024];
    int recv_len = 0;

    addr_len = sizeof(client_addr);
    accept_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &addr_len);
    if (accept_fd < 0) {
        printf("error to accept\n");
    }
    while (1) {
        recv_len = read(accept_fd, buf, sizeof(buf) - 1);
        if (recv_len < 0) {
            printf("error to read\n");
            exit(0);
        }
        buf[recv_len] = '\0';
        printf("recv buf: %s\n", buf);
    }
    return 0;
}
