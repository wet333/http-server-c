#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H

#include <sys/socket.h>

// HTTP Methods
typedef enum {
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_PUT,
    HTTP_METHOD_DELETE,
    HTTP_METHOD_PATCH,
    HTTP_METHOD_HEAD,
    HTTP_METHOD_OPTIONS,
    HTTP_METHOD_UNKNOWN
} HttpMethod;

// HTTP Request structure
typedef struct {
    HttpMethod method;
    char *path;
    char *raw_request;
    size_t raw_request_size;
} HttpRequest;

// Function declarations
int read_request(int client_socket, HttpRequest *req, char *buffer, size_t buffer_size);
HttpMethod parse_method(const char *request_line);
int parse_path(const char *request_line, char *path_buffer, size_t path_buffer_size);
void free_request(HttpRequest *req);

#endif
