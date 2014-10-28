#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void sys_error(char *str)
{
    printf("error: %s\n", str);
    exit(1);
}

/*
 * init tcp socket
 */
int init_tcp(int is_server, char *ip_addr, int port)
{
    int ret = -1;

    if (is_server) {
        ret = init_tcp_server(ip_addr, port);
    } else {
        ret = init_tcp_client(ip_addr, port);
    }

    return ret;
}

/*
 * function: init tcp server
 */
int init_tcp_server(char *ip_addr, int port)
{
    int ret = -1;
    int sock_fd = -1;
    struct sockaddr_in server_addr;
    socklen_t addr_len = 0;
    int optval = 1;

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        sys_error("fail to socket in creating tcp-server");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip_addr);

    ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (ret < 0) {
        sys_error("fail to setsockopt\n");
    }
    ret = bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (ret < 0) {
        sys_error("fail to bind in creating tcp-server");
    }

    ret = listen(sock_fd, 20);
    if (ret < 0) {
        sys_error("fail to listen in creating tcp-server");
    }

    return sock_fd;
}

/*
 * function: init tcp client
 */
int init_tcp_client(char *ip_addr, int port)
{
    int ret = -1;
    int sock_fd = -1;
    int optval = 1;
    struct sockaddr_in client_addr;

    printf("ip_addr: %s, %d\n", ip_addr, port);
    if (ip_addr == NULL && port < 1024) {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            sys_error("fail to socket in creating client");
        }
    } else {
        sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (sock_fd < 0) {
            sys_error("fail to socket in creating client");
        }

        memset(&client_addr, 0, sizeof(client_addr));
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(port);
        client_addr.sin_addr.s_addr = inet_addr(ip_addr);

        ret = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
        if (ret < 0) {
            sys_error("fail to setsockopt\n");
    }
#if 0
        ret = bind(sock_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
        if (ret < 0) {
            sys_error("fail to bind in creating client");
        }
#endif
    }
    printf("sock fd after bind : %d\n", sock_fd);
    return sock_fd;
}


/*
 * init socket for UDP or TCP
 */
int socket_init(int is_server, int socket_type, char *ip_addr, int port)
{
    int ret = -1;

    if (socket_type == SOCK_STREAM) {
        ret = init_tcp(is_server, ip_addr, port);
    } else {
        //TODO
    }

    return ret;
}

void test()
{
    printf("test in ar\n");
}
