#include <arpa/inet.h> // For internet address manipulation
#include <bits/types/struct_timeval.h>
#include <netinet/in.h> // For sockaddr_in and related structures
#include <signal.h>
#include <signal.h>     // For signal handling
#include <stdio.h>      // For standard input/output operations
#include <stdlib.h>     // For memory allocation and exit functions
#include <string.h>     // For string manipulation functions
#include <sys/select.h> // Used for select concurrency
#include <sys/socket.h> // For socket functions
#include <sys/types.h>  // For types used in socket programming
#include <unistd.h>     // For close() function

// Include custom headers for parsing request and generating response
#include "./request-parser/request-parser.h"
#include "./response-generator/response-generator.h"

// Define buffer size and port number for the server
#define buf_size 1024
#define port 8080

static volatile int keepRunning = 1; // Boolean to keep track of running state
void intHandler(int) {
  keepRunning = 0;
} // If there is an interrupt (Ctrl+C) set keep running to zero

int main(int argc, char *argv[]) {
  // Declare buffers for receiving and sending data
  char receiving_buffer[buf_size];
  char sending_buffer[buf_size];
  int bytes_received;
  int bytes_sent;

  // Declare server and client socket descriptors and address structures
  int serverfd, clientfd;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  struct timeval timeout;
  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  // Create a socket for the server (IPv4, TCP)
  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd == -1) { // Check if socket creation was successful
    perror("socket");
    return 1;
  }
  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets);
  FD_SET(serverfd, &current_sockets);

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

  signal(SIGINT, intHandler); // Assign the inthandler to interrupts.
  // Main server loop
  while (keepRunning) {

    ready_sockets = current_sockets;

    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &timeout) < 0) {
      perror("select");
      return 1;
    }

    for (int i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == serverfd) {
          // Accept a new client connection
          clientfd = accept(serverfd, (struct sockaddr *)&client_addr,
                            &client_addr_len);
          if (clientfd <
              0) { // Check if accepting the connection was successful
            perror("accept");
            return 1;
          }
          FD_SET(clientfd, &current_sockets);
        } else {
          // Receive the HTTP request from the client
          bytes_received = recv(i, receiving_buffer, buf_size, 0);
          if (bytes_received ==
              -1) { // Check if receiving the data was successful
            perror("recv");
            close(serverfd);
            close(i);
            return 1;
          }

          // Parse the HTTP request using parse_request
          struct http_request *request = parse_request(receiving_buffer);

          // Generate a response based on the parsed request
          struct http_response *response = generate_response(request);

          // Send the response headers to the client
          if (send(i, response->headers, strlen(response->headers), 0) < 0) {
            perror("send");
            close(serverfd);
            close(i);
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
          close(i);

          // Free allocated memory for the request and response structs
          free(request);
          free(response);

          FD_CLR(i, &current_sockets);
        }
      }
    }
  }

  // Close the server socket before exiting
  close(serverfd);
  return 0;
}
