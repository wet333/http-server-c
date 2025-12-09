#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

typedef struct {
    int socket_fd;
    int port;
    struct sockaddr_in address;
} HttpServer;

HttpServer init_server(int port);

#endif