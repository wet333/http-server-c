#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// Envía un archivo completo a un socket cliente
void send_file(int client_socket, const char *filename);

#endif