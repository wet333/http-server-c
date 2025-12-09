#include "file_handler.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

void send_file(int client_socket, const char *filename) {

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Basic HTTP Headers
    char *headers = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\n\r\n";
    send(client_socket, headers, strlen(headers), 0);

    // Send file by chunks
    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
        send(client_socket, buffer, bytes_read, MSG_NOSIGNAL);
    }

    fclose(file);
    printf("File %s sent.\n", filename);
}