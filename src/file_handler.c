#include "file_handler.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "constants.h"
#include "http_response.h"
#include "utils.h"

// Send file to client socket
void send_file(int client_socket, char *filename) {

    HttpResponse res;
    char response_buffer[BUFFER_SIZE];

    printf("File to open: %s \n", filename);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");

        // Not found - Response
        response_init(&res, HTTP_NOT_FOUND, MIME_APPLICATION_JSON);
        response_set_body(&res, "{\"Error 404\": \"File Not found.\"}");

        int total_bytes = get_response_size(&res, response_buffer, sizeof(response_buffer));

        send(client_socket, response_buffer, total_bytes, 0);
        return;
    }

    // Basic HTTP Headers
    response_init(&res, HTTP_OK, MIME_TEXT_HTML);
    res.body_length = get_file_size(filename);
    response_add_header(&res, "Server-Token", "987654321"); // Test header (no real use)
    int total_bytes = get_response_size(&res, response_buffer, sizeof(response_buffer));

    // Send Headers
    send(client_socket, response_buffer, total_bytes, 0);

    // Send file by chunks
    stream_file_to_client(client_socket, filename);

    fclose(file);
    printf("File %s sent.\n", filename);
}

long get_file_size(char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fclose(fp);

    return size;
}

void stream_file_to_client(int client_socket, char *filename) {
    FILE *fp = fopen(filename, "rb");
    char file_buffer[BUFFER_SIZE];
    size_t bytes_read;

    while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(client_socket, file_buffer, bytes_read, 0);
    }

    fclose(fp);
}