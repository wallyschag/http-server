#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>

#include "../logger/logger.h"
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

  char client_ip_string[64];
  inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip_string,
            sizeof(client_ip_string));
  log_message("Connection accepted on client socket #%d from %s", clientfd,
              client_ip_string);
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

  log_message("Recieved %d bytes on socket %d", bytes_received, clientfd);
  log_message("Request received:\n%s", receiving_buffer);

  // Parse the receiving buffer and create a http_request struct
  log_message("Parsing request...");
  struct http_request *request = parse_request(receiving_buffer);
  log_message("Request parsed!");

  // Create an http_response struct based on the http_request struct
  log_message("Generating response based on parsed request content...");
  struct http_response *response = generate_response(request);
  log_message("Response generated!");

  // Send necessary headers to the client. Included in http_response sttuct;
  log_message("Sending response headers...");
  if (send(clientfd, response->headers, strlen(response->headers), 0) < 0) {
    perror("send");
    close(clientfd);
    return 1;
  }

  log_message("Sent response headers from server on client sokcet #%d\n%s",
              clientfd, response->headers);

  // If the requested file exists, send the file
  log_message("Payload object requested by client: %s", response->target_file);
  if (response->target_file != NULL) {
    log_message("Sending payload to client.");
    while ((bytes_sent = fread(sending_buffer, 1, BUF_SIZE,
                               response->target_file)) > 0) {
      log_message("%d payload bytes successfully sent to client.", bytes_sent);
      send(clientfd, sending_buffer, bytes_sent, 0);
    }
    fclose(response->target_file);
  }

  // Close client socket
  log_message("Closing client socket #%d", clientfd);
  close(clientfd);

  // Free dynamically allocated structs
  free(request);
  free(response);

  // Return 0 on success
  return 0;
}
