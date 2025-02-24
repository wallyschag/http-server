#include <netinet/in.h>
#include <stdio.h>

#include "../request-parser/request-parser.h"
#include "../response-generator/response-generator.h"
#include "server-processing.h"

#define BUF_SIZE 1024

int accept_connection(int serverfd) {
  // Initialize client file descriptor and address stucture
  int clientfd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // Accept connection on sever socket and set that equal to client socket
  clientfd =
      accept(serverfd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (clientfd < 0) {
    perror("accept");
    exit(1);
  }
  return clientfd;
}

int process_connection(int clientfd) {
  // Define receiving and sending buffer
  char receiving_buffer[BUF_SIZE];
  char sending_buffer[BUF_SIZE];

  // Define number of bytes received and sent
  int bytes_received;
  int bytes_sent;

  // Receive request from client and put that in the receiving buffer
  bytes_received = recv(clientfd, receiving_buffer, BUF_SIZE, 0);
  if (bytes_received == -1) {
    perror("recv");
    close(clientfd);
    return 1;
  }

  // Parse the receiving buffer and create a http_request struct
  struct http_request *request = parse_request(receiving_buffer);

  // Create an http_response struct based on the http_request struct
  struct http_response *response = generate_response(request);

  // Send necessary headers to the client. Included in http_response sttuct;
  if (send(clientfd, response->headers, strlen(response->headers), 0) < 0) {
    perror("send");
    close(clientfd);
    return 1;
  }

  // If the requested file exists, send the file
  if (response->target_file != NULL) {
    while ((bytes_sent = fread(sending_buffer, 1, BUF_SIZE,
                               response->target_file)) > 0) {
      send(clientfd, sending_buffer, bytes_sent, 0);
    }
    fclose(response->target_file);
  }

  // Close client socket
  close(clientfd);

  // Free dynamically allocated structs
  free(request);
  free(response);

  // Return 0 on success
  return 0;
}
