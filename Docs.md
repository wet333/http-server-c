# Nano Server C - Technical Documentation

## Table of Contents
1. [Architecture Overview](#architecture-overview)
2. [Project Organization](#project-organization)
3. [Components and Functions](#components-and-functions)

---

## Architecture Overview

### How the Server Works

Nano Server C is a simple HTTP server implementation written in C that uses **socket programming** to handle client connections. The server follows a traditional **request-response model** where it listens for incoming connections, processes requests, and sends back HTTP responses.

#### High-Level Flow

1. **Initialization Phase**
   - The server creates a TCP socket using the `socket()` system call
   - Binds the socket to a specific port (default: 8080)
   - Starts listening for incoming connections using `listen()`

2. **Connection Handling**
   - The server enters an infinite loop waiting for client connections
   - When a client connects, `accept()` creates a new socket for that specific connection
   - The server reads the client's HTTP request (currently ignored in the implementation)
   - Processes the request and generates an appropriate HTTP response
   - Sends the response back to the client
   - Closes the connection

3. **Response Generation**
   - The server constructs HTTP responses following the HTTP/1.1 protocol
   - Responses include status lines, headers, and body content
   - Files are streamed to clients in chunks to handle large files efficiently

#### Key Architectural Decisions

- **Single-threaded**: The server handles one connection at a time in a blocking manner
- **Connection-per-request**: Each client connection is closed after sending the response
- **File-based responses**: Currently serves static files from the `bin/` directory
- **Modular design**: Separates concerns into distinct modules (server, HTTP, file handling)

---

## Project Organization

### Directory Structure

```
nano_server_c/
├── src/              # Source code files (.c)
│   ├── main.c        # Entry point and main event loop
│   ├── server.c      # Server initialization and socket setup
│   ├── http_response.c  # HTTP response construction
│   ├── file_handler.c   # File operations and streaming
│   └── utils.c       # Utility functions
│
├── include/          # Header files (.h)
│   ├── server.h      # Server structure and functions
│   ├── http_response.h  # HTTP response types and functions
│   ├── file_handler.h   # File handling function declarations
│   ├── constants.h      # Global constants and macros
│   └── utils.h       # Utility function declarations
│
├── build/            # Compiled object files (.o)
│
├── bin/              # Binary output and static files
│   ├── server        # Compiled executable
│   └── index.html    # Example HTML file served by the server
│
├── Makefile          # Build configuration
└── Docs.md          # This documentation file
```

### Build System

The project uses a **Makefile** for compilation:

- **Source files** (`src/*.c`) are compiled into **object files** (`build/*.o`)
- Object files are **linked** together to create the final executable (`bin/server`)
- The Makefile automatically discovers all `.c` files in the `src/` directory
- Compiler flags include: `-Iinclude` (header search path), `-Wall` (warnings), `-g` (debug symbols)

**Build commands:**
- `make` or `make all` - Compiles the project
- `make clean` - Removes build artifacts

---

## Components and Functions

### 1. Main Module (`src/main.c`)

The entry point of the application. Contains the main event loop that keeps the server running.

#### `main()`
- **Purpose**: Entry point that initializes the server and runs the main event loop
- **Flow**:
  1. Initializes the HTTP server on port 8080
  2. Enters an infinite loop waiting for client connections
  3. Accepts new connections using `accept()`
  4. Reads the client request (currently stored in a temporary buffer)
  5. Sends a file response using `send_file()`
  6. Closes the client socket
- **Note**: Currently ignores the actual request content and always sends `index.html`

---

### 2. Server Module (`src/server.c`, `include/server.h`)

Handles the low-level socket operations and server initialization.

#### `HttpServer` Structure
```c
typedef struct {
    int socket_fd;              // File descriptor for the server socket
    int port;                   // Port number the server listens on
    struct sockaddr_in address; // Socket address structure
} HttpServer;
```

#### `init_server(int port)`
- **Purpose**: Initializes and configures the HTTP server socket
- **Parameters**: 
  - `port`: The port number to bind the server to
- **Returns**: An `HttpServer` structure containing the configured server
- **Process**:
  1. Creates a TCP socket using `socket(AF_INET, SOCK_STREAM, 0)`
  2. Sets socket options (SO_REUSEADDR) to allow port reuse
  3. Configures the address structure (IPv4, any interface, specified port)
  4. Binds the socket to the address using `bind()`
  5. Starts listening for connections with `listen()` (backlog of 3)
- **Error Handling**: Exits the program if any step fails

---

### 3. HTTP Response Module (`src/http_response.c`, `include/http_response.h`)

Manages HTTP response construction and serialization according to HTTP/1.1 standards.

#### Types and Enumerations

**`HttpStatus`** - HTTP status codes:
- `HTTP_OK` (200) - Successful request
- `HTTP_CREATED` (201) - Resource created
- `HTTP_BAD_REQUEST` (400) - Invalid request
- `HTTP_UNAUTHORIZED` (401) - Authentication required
- `HTTP_FORBIDDEN` (403) - Access denied
- `HTTP_NOT_FOUND` (404) - Resource not found
- `HTTP_INTERNAL_ERROR` (500) - Server error

**`MimeType`** - Content type identifiers:
- `MIME_TEXT_HTML` - HTML content
- `MIME_TEXT_PLAIN` - Plain text
- `MIME_APPLICATION_JSON` - JSON data
- `MIME_IMAGE_PNG` - PNG images

**`HttpHeader`** - Key-value pair for custom headers:
```c
typedef struct {
    char *key;
    char *value;
} HttpHeader;
```

**`HttpResponse`** - Complete HTTP response structure:
```c
typedef struct {
    HttpStatus status;              // HTTP status code
    MimeType content_type;         // Content-Type header value
    HttpHeader extra_headers[20];   // Array for custom headers
    int header_count;              // Number of custom headers added
    char *body;                    // Response body content
    size_t body_length;            // Length of body in bytes
} HttpResponse;
```

#### Functions

**`get_status_phrase(HttpStatus status)`**
- **Purpose**: Converts an HTTP status code enum to its corresponding status phrase
- **Returns**: String like "OK", "Not Found", "Internal Server Error", etc.
- **Example**: `HTTP_OK` → `"OK"`

**`get_mime_string(MimeType type)`**
- **Purpose**: Converts a MIME type enum to its full MIME type string
- **Returns**: String like "text/html; charset=UTF-8", "application/json", etc.
- **Example**: `MIME_TEXT_HTML` → `"text/html; charset=UTF-8"`

**`response_init(HttpResponse *res, HttpStatus status, MimeType type)`**
- **Purpose**: Initializes a new HTTP response structure with default values
- **Parameters**:
  - `res`: Pointer to the response structure to initialize
  - `status`: HTTP status code
  - `type`: MIME type for the Content-Type header
- **Effects**: Sets status, content type, clears headers, and sets body to NULL

**`response_add_header(HttpResponse *res, char *key, char *value)`**
- **Purpose**: Adds a custom HTTP header to the response
- **Parameters**:
  - `res`: Pointer to the response structure
  - `key`: Header name (e.g., "Server-Token")
  - `value`: Header value
- **Limitation**: Maximum of 20 custom headers per response
- **Note**: Headers are stored as pointers, so the strings must remain valid

**`response_set_body(HttpResponse *res, char *body)`**
- **Purpose**: Sets the response body content
- **Parameters**:
  - `res`: Pointer to the response structure
  - `body`: Pointer to the body content string
- **Effects**: Sets body pointer and calculates length using `strlen()`
- **Note**: Currently works for text content only (uses `strlen()`, not suitable for binary data)

**`response_serialize(HttpResponse *res, char *buffer, size_t buffer_size)`**
- **Purpose**: Converts the HTTP response structure into a formatted HTTP message string
- **Parameters**:
  - `res`: Pointer to the response structure
  - `buffer`: Character buffer to write the serialized response into
  - `buffer_size`: Maximum size of the buffer
- **Returns**: Total number of bytes written to the buffer
- **Process**:
  1. Writes the HTTP status line: `HTTP/1.1 <status> <phrase>\r\n`
  2. Writes standard headers: Content-Type, Content-Length, Connection, Server
  3. Writes all custom headers added via `response_add_header()`
  4. Writes the empty line separator (`\r\n`)
  5. Writes the response body (if present)
- **Format**: Follows HTTP/1.1 specification with `\r\n` line endings

---

### 4. File Handler Module (`src/file_handler.c`, `include/file_handler.h`)

Handles file operations, including reading files and streaming them to clients.

#### `send_file(int client_socket, char *filename)`
- **Purpose**: Sends a file to a client socket with proper HTTP response headers
- **Parameters**:
  - `client_socket`: File descriptor of the client connection
  - `filename`: Path to the file to send
- **Process**:
  1. Attempts to open the file
  2. If file not found: Creates a 404 error response and sends it
  3. If file found: 
     - Initializes a 200 OK response with HTML content type
     - Calculates file size
     - Adds custom headers (currently includes a test "Server-Token" header)
     - Serializes and sends the HTTP headers
     - Streams the file content in chunks
  4. Closes the file handle
- **Error Handling**: Sends a JSON error response if file cannot be opened

#### `get_file_size(char *filename)`
- **Purpose**: Calculates the size of a file in bytes
- **Parameters**: `filename` - Path to the file
- **Returns**: File size in bytes, or -1 if the file cannot be opened
- **Implementation**: Opens file in binary mode, seeks to end, uses `ftell()` to get position

#### `stream_file_to_client(int client_socket, char *filename)`
- **Purpose**: Streams a file to a client socket in chunks
- **Parameters**:
  - `client_socket`: File descriptor of the client connection
  - `filename`: Path to the file to stream
- **Process**:
  1. Opens the file in binary read mode
  2. Reads the file in chunks of `BUFFER_SIZE` bytes (4096 bytes)
  3. Sends each chunk immediately to the client using `send()`
  4. Continues until the entire file is read
  5. Closes the file
- **Benefits**: Handles large files efficiently without loading entire file into memory

---

### 5. Utils Module (`src/utils.c`, `include/utils.h`)

Provides utility functions used across the codebase.

#### `long_to_string(long number)`
- **Purpose**: Converts a long integer to a dynamically allocated string
- **Parameters**: `number` - The long integer to convert
- **Returns**: Pointer to a newly allocated string containing the number, or NULL on failure
- **Memory Management**: Allocates memory using `malloc()`. **Caller is responsible for freeing** the returned string
- **Implementation**: Uses `snprintf()` to calculate required size, then allocates and formats

---

### 6. Constants Module (`include/constants.h`)

Defines global constants and configuration values used throughout the application.

#### Constants

**`BUFFER_SIZE`** (4096)
- Default buffer size in bytes for reading files and network operations
- Used for temporary buffers when reading client requests and streaming files

**`SERVER_NAME`** ("AWetServerV1.0")
- Server identification string sent in the "Server" HTTP header
- Identifies the server software to clients

---

## Request-Response Flow Example

Here's a step-by-step example of what happens when a client connects:

1. **Client connects** to `localhost:8080`
2. **Server accepts** the connection (`accept()`)
3. **Server reads** the HTTP request (currently ignored)
4. **Server calls** `send_file(client_socket, "index.html")`
5. **File handler** opens `index.html` from the `bin/` directory
6. **HTTP response** is initialized with status 200 OK and HTML content type
7. **Response headers** are serialized and sent to the client
8. **File content** is streamed in 4KB chunks
9. **Connection is closed** by the server
10. **Server returns** to waiting for the next connection

---

## Current Limitations and Future Extensions

### Known Limitations

1. **Single-threaded**: Only handles one connection at a time
2. **Request parsing**: Client requests are read but not parsed or used
3. **Static responses**: Always serves `index.html` regardless of request
4. **No routing**: No URL path handling or routing mechanism
5. **Binary file support**: Body length calculation uses `strlen()`, which doesn't work for binary files
6. **Memory management**: Some allocated memory (e.g., from `long_to_string()`) may not be freed

### Potential Extensions

- **Request parsing**: Parse HTTP request method, path, headers
- **Routing system**: Map URL paths to different handlers or files
- **Multi-threading**: Use threads or processes to handle multiple clients concurrently
- **Configuration file**: Allow port, directory, and other settings to be configured
- **Logging system**: Add structured logging for requests and errors
- **Error pages**: Custom error pages for different error conditions
- **HTTP methods**: Support for POST, PUT, DELETE, etc.
- **Static file serving**: Serve files from a configurable directory based on request path

---

## Compilation and Execution

### Building the Server

```bash
make
```

This will:
- Create `build/` and `bin/` directories if they don't exist
- Compile all `.c` files in `src/` to object files in `build/`
- Link object files to create `bin/server`

### Running the Server

```bash
./bin/server
```

The server will start listening on port 8080. You can test it by:
- Opening `http://localhost:8080` in a web browser
- Using `curl http://localhost:8080`

### Cleaning Build Artifacts

```bash
make clean
```

Removes the `build/` and `bin/` directories.

---

## Dependencies

- **Standard C Library**: `<stdio.h>`, `<stdlib.h>`, `<string.h>`
- **POSIX Sockets**: `<sys/socket.h>`, `<netinet/in.h>`
- **POSIX I/O**: `<unistd.h>`

No external libraries are required beyond the standard C library and POSIX system calls.

---

*Last Updated: Based on current codebase structure*
