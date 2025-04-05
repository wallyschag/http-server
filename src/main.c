#include <arpa/inet.h>                 // For internet address manipulation
#include <bits/types/struct_timeval.h> // Handle timeval functionality for select
#include <signal.h>                    // For signal handling
#include <stdio.h>                     // For standard input/output operations
#include <sys/select.h>                // Used for select concurrency
#include <sys/socket.h>                // For socket functions
#include <sys/types.h>                 // For types used in socket programming
#include <unistd.h>                    // For close() function

#include "logger/logger.h" //System logging. Log everything to log.txt
#include "server-processing/server-processing.h" // Include custom server processing functions
#define PORT 8080                                // Define port here

// Main loop boolean and Signal handling function for interrupts
// Sets boolean to false upon interrupt to close cleanly
static volatile int running = 1;
void intHandler(int signal) {
  signal = signal;
  running = 0;
}

int main() {
  log_message("################################################");
  log_message("Server initialized.");
  // Define server and client file descriptors
  int serverfd, clientfd;

  // Create a TCP server socket based on the previously defined server fd
  serverfd = socket(AF_INET, SOCK_STREAM, 0);
  if (serverfd == -1) {
    perror("socket");
    return 1;
  }

  log_message("Server Socket #%d created.", serverfd);

  // Define server address structure
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  // Bind server socket to server address structure
  if (bind(serverfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) {
    perror("bind");
    return 1;
  }

  log_message("Server socket #%d bound to port %d.", serverfd, PORT);

  // Listen for incoming connections on server socket
  if (listen(serverfd, 5) == -1) {
    perror("listen");
    return 1;
  }
  log_message("Serevr listening on port %d", PORT);

  // Bind the interrupt signal with the interrupt handler function defined above
  // main
  signal(SIGINT, intHandler);

  // Define current and ready file descriptors for select
  fd_set current_sockets, ready_sockets;
  FD_ZERO(&current_sockets); // Clear out current sockets
  FD_SET(serverfd,
         &current_sockets); // Add the server socket into current sockets set
  struct timeval timeout = {5, 0}; // Set timeout for select to five seconds

  // Main server while loop
  // running gets set to false if an interrupt (ctrl + c) gets detected
  while (running) {
    // Copy the current sockets to the ready sockets
    ready_sockets = current_sockets;

    // Select active sockets in the ready sockets set
    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, &timeout) < 0) {
      perror("select");
      return 1;
    }

    // Loop through active sockets
    for (int i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == serverfd) {
          // If the server socket is active,
          // Accept connection and create client socket
          // Add client socket to current sockets set
          clientfd = accept_connection(i);
          FD_SET(clientfd, &current_sockets);
        } else {
          // The current active socket is a client socket
          // Process connection -> Parse and send response
          // Remove current socket from current sockets
          if (process_connection(i) == 1) {
            printf("Error in processing the connection.");
            close(serverfd);
            return 1;
          }
          FD_CLR(i, &current_sockets);
        }
      }
    }
  } // While loop

  // Close server socket and close
  close(serverfd);
  log_message("Server socket #%d closed.", serverfd);
  log_message("Server operations aborted. Closing...");
  return 0;
}
