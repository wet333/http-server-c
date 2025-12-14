#ifndef HTTP_RESPONSE_H
#define HTTP_RESPONSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum {
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_INTERNAL_ERROR = 500
} HttpStatus;

typedef enum {
    MIME_TEXT_HTML,
    MIME_TEXT_PLAIN,
    MIME_APPLICATION_JSON,
    MIME_IMAGE_PNG
} MimeType;

typedef struct {
    char *key;
    char *value;
} HttpHeader;

typedef struct {
    HttpStatus status;
    MimeType content_type;
    
    HttpHeader extra_headers[20]; 
    int header_count;
    
    char *body;
    size_t body_length;
} HttpResponse;

const char* get_status_phrase(HttpStatus status);
const char* get_mime_string(MimeType type);
void response_init(HttpResponse *res, HttpStatus status, MimeType type);
void response_add_header(HttpResponse *res, char *key, char *value);
void response_set_body(HttpResponse *res, char *body);
int get_response_size(HttpResponse *res, char *buffer, size_t buffer_size);

#endif