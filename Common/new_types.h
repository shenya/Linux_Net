#ifndef _NEW_TYPES_H
#define _NEW_TYPES_H

#define IP_ADDR_LEN 32
typedef struct net_info {
    char local_ip_addr[IP_ADDR_LEN];
    char server_ip_addr[IP_ADDR_LEN];
    int local_port;
    int server_port;
}NET_INFO;

#endif
