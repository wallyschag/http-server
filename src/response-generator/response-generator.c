#include <stdio.h>  // For standard input/output functions
#include <stdlib.h> // For memory allocation and exit functions
#include <string.h> // For string manipulation functions

// Include custom headers for parsing requests and generating responses
#include "./response-generator.h"

// Generate an http_response struct by parsing a http_request struct
struct http_response *generate_response(struct http_request *request) {

  // Allocate memory for the HTTP response struct
  struct http_response *response = malloc(sizeof(struct http_response));
  if (response == NULL) { // Check for memory allocation failure
    perror("Memory allocation");
    exit(1);
  }

  char header_buf[1000];
  response->headers = header_buf;
  header_buf[0] = '\0';

  // Check if the request method is "GET"
  if (!strcmp(request->method, "GET")) {
    // Adjust the request target (remove leading '/' for file path)
    request->request_target = request->request_target + 1;

    // Attempt to open the requested file
    FILE *file = fopen(request->request_target, "r");
    if (!file) { // If the file is not found
      // Set response headers to 404 Not Found and no file to send
      snprintf(header_buf, sizeof(header_buf),
               "HTTP/1.1 404 Not Found\r\n"
               "Content-Type: text/html\t\n\r\n");

      response->target_file = NULL;
    } else {
      // If the file exists, set response headers to 200 OK and send the file
      snprintf(header_buf, sizeof(header_buf),
               "HTTP/1.1 200 OK\r\n"
               "Content-Type: %s",
               request->content_type);
      response->target_file = file;
    }
  } else { // If the method is not "GET"
           // Set response headers to 405 Method Not Allowed
    snprintf(
        header_buf, sizeof(header_buf),
        "HTTP/1.1 405 Method Not Allowed\r\nContent-Type: text/html\t\n\r\n");
    response->target_file = NULL; // No file to send
  }

  // TODO: Handle other HTTP methods such as HEAD, POST, PUT, TRACE...

  // Return the generated response
  return response;
}
