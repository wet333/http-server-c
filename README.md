# Nano Server C

A lightweight HTTP server written in C for serving static files. A minimal implementation demonstrating socket programming and HTTP protocol handling.

## How It Works

The server uses POSIX sockets to create a TCP server listening on port 8080. When a client connects, it parses incoming HTTP requests, extracts the requested file path, and serves files from the server's directory structure. The server handles HTTP response generation with proper headers (including Content-Type detection based on file extensions), streams large files in chunks for efficiency, and returns appropriate status codes (200 OK for successful requests, 404 Not Found for missing files).

## Building

To build the server executable:

```bash
make
```

This will compile all source files in the `src/` directory and create the executable at `bin/server`. The build process creates object files in the `build/` directory.

To clean build artifacts:

```bash
make clean
```

## Usage

### Starting the Server

Run the server executable:

```bash
./bin/server
```

The server will start listening on port 8080. You should see "Listening for connections..." in the console.

### Serving Files

Once running, access files via HTTP requests. The server serves files relative to its working directory (typically the project root). For example:

- `http://localhost:8080/bin/index.html` - Serves the index page
- `http://localhost:8080/bin/styles.css` - Serves the CSS file
- `http://localhost:8080/bin/pages/tutorial.html` - Serves nested pages
- `http://localhost:8080/ZI_00035_.png` - Serves images (.png only)

### Capabilities

- **HTTP Methods**: Supports GET, POST, PUT, DELETE, PATCH, HEAD, and OPTIONS requests
- **MIME Type Detection**: Automatically sets Content-Type headers for HTML, CSS, JSON, PNG, and plain text files
- **File Streaming**: Efficiently streams large files in 4KB chunks
- **Error Handling**: Returns 404 Not Found responses for missing files
- **Request Parsing**: Parses HTTP request headers and extracts the requested path
- **Connection Management**: Handles multiple sequential client connections
