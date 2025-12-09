#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "server.h"
#include "file_handler.h"

int main() {
    // Init Server Socket
    HttpServer server = init_server(8080);
    
    int new_socket;
    int addrlen = sizeof(server.address);

    // Main Loop
    while (1) {
        printf("\nWaiting for connections...\n");
        
        // Client socket connection
        new_socket = accept(
            server.socket_fd, 
            (struct sockaddr *)&server.address, 
            (socklen_t*)&addrlen
        );
                           
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        // We ignore the client request by now
        char temp_buffer[1024];
        recv(new_socket, temp_buffer, 1024, 0);

        // Send response
        send_file(new_socket, "index.html");

        close(new_socket);
    }

    return 0;
}