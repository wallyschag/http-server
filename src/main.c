#include <arpa/inet.h>  // For internet address manipulation
#include <netinet/in.h> // For sockaddr_in and related structures
#include <stdio.h>      // For standard input/output operations
#include <stdlib.h>     // For memory allocation and exit functions
#include <string.h>     // For string manipulation functions
#include <sys/socket.h> // For socket functions
#include <sys/types.h>  // For types used in socket programming
#include <unistd.h>     // For close() function

// Include custom headers for parsing request and generating response
#include "./request-parser/request-parser.h"
#include "./response-generator/response-generator.h"

// Define buffer size and port number for the server
#define buf_size 1024
#define port 8080

int main(int argc, char *argv[]) {
  // Declare buffers for receiving and sending data
  char receiving_buffer[buf_size];
  char sending_buffer[buf_size];
  size_t bytes_received;
  size_t bytes_sent;

  // Declare server and client socket descriptors and address structures
  int serverfd;
  int clientfd;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // Create a socket for the server (IPv4, TCP)
  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd == -1) { // Check if socket creation was successful
    perror("socket");
    return 1;
  }

  // Initialize server address structure
  server_addr.sin_family = AF_INET; // IPv4 address family
  server_addr.sin_port =
      htons(port); // Set port number (converted to network byte order)
  server_addr.sin_addr.s_addr =
      INADDR_ANY; // Bind to any available network interface

  // Bind the socket to the server address
  if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
    perror("bind");
    return 1;
  }

  // Start listening for incoming client connections
  if (listen(serverfd, 5) == -1) {
    perror("listen");
    return 1;
  }

  // Main server loop
  while (1) {
    // Accept a new client connection
    clientfd =
        accept(serverfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (clientfd < 0) { // Check if accepting the connection was successful
      perror("accept");
      return 1;
    }

    // Receive the HTTP request from the client
    bytes_received = recv(clientfd, receiving_buffer, buf_size, 0);
    if (bytes_received == -1) { // Check if receiving the data was successful
      perror("recv");
      close(serverfd);
      close(clientfd);
      return 1;
    }

    // Parse the HTTP request using parse_request
    struct http_request *request = parse_request(receiving_buffer);

    // Generate a response based on the parsed request
    struct http_response *response = generate_response(request);

    // Send the response headers to the client
    if (send(clientfd, response->headers, strlen(response->headers), 0) < 0) {
      perror("send");
      close(clientfd);
      return 1;
    }

    // Send the response body (if any file is specified in the response)
    if (response->target_file != NULL) {
      while ((bytes_sent = fread(sending_buffer, 1, buf_size,
                                 response->target_file)) > 0) {
        send(clientfd, sending_buffer, bytes_sent,
             0); // Send file contents to client
      }
      fclose(response->target_file); // Close the file after sending
    }

    // Close the client connection after processing the request
    close(clientfd);

    // Free allocated memory for the request and response structs
    free(request);
    free(response);
  }

  // Close the server socket before exiting
  close(serverfd);
  return 0;
}
