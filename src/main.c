#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include "server.h"
#include "file_handler.h"
#include "constants.h"
#include "http_request.h"

int main(int argc, char *argv[]) {

    // // Argument checks
    // if (argc < 2) {
    //     fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    //     return 1;
    // }

    // char *response_file = argv[1]; // Configure file tu return

    // Init Server Socket
    HttpServer server = init_server(8080);
    
    int new_socket;
    int addrlen = sizeof(server.address);

    // Main Loop
    while (1) {

        printf("Listening for connections...");

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

        // Read and parse the HTTP request
        char request_buffer[BUFFER_SIZE];
        HttpRequest request;
        
        int bytes_read = read_request(new_socket, &request, request_buffer, sizeof(request_buffer));
        
        if (bytes_read > 0) {
            // Print parsed request information
            const char *method_str = "UNKNOWN";
            switch (request.method) {
                case HTTP_METHOD_GET: method_str = "GET"; break;
                case HTTP_METHOD_POST: method_str = "POST"; break;
                case HTTP_METHOD_PUT: method_str = "PUT"; break;
                case HTTP_METHOD_DELETE: method_str = "DELETE"; break;
                case HTTP_METHOD_PATCH: method_str = "PATCH"; break;
                case HTTP_METHOD_HEAD: method_str = "HEAD"; break;
                case HTTP_METHOD_OPTIONS: method_str = "OPTIONS"; break;
                default: method_str = "UNKNOWN"; break;
            }
            
            // Log HTTP method
            printf("Request Method: %s\n", method_str);

            // Log Request PATH
            if (request.path != NULL) {
                printf("Request Path: %s\n", request.path);
            }

            // Send requested file
            send_file(new_socket, request.path);
            
            // Clean up request memory
            free_request(&request);

        } else if (bytes_read == 0) {
            printf("Client closed connection\n");
            close(new_socket);
            continue;
        } else {
            printf("Error reading request\n");
            close(new_socket);
            continue;
        }

        close(new_socket);
    }

    return 0;
}