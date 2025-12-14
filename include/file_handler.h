#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

void send_file(int client_socket, char *filename);
long get_file_size(char *filename);
void stream_file_to_client(int client_socket, char *filename);

#endif