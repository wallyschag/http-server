#include <stdio.h>  // For standard input/output functions
#include <stdlib.h> // For memory allocation and exit functions
#include <string.h> // For string manipulation functions

// Include custom headers for parsing requests and generating responses
#include "./response-generator.h"

/**
 * @brief Generates an HTTP response based on the HTTP request.
 *
 * This function takes a parsed HTTP request and generates an HTTP response.
 * It handles the "GET" method by attempting to open the requested file and
 * returns a corresponding HTTP status code. If the method is unsupported,
 * it returns a "405 Method Not Allowed" status.
 *
 * @param request A pointer to the HTTP request to process.
 * @return A pointer to an allocated http_response struct containing headers
 * and the target file (if any).
 *
 * @note This function only handles the "GET" method and is set up for future
 * expansion to handle other HTTP methods (e.g., POST, PUT).
 */
struct http_response *generate_response(struct http_request *request) {

  // Allocate memory for the HTTP response struct
  struct http_response *response = malloc(sizeof(struct http_response));
  if (response == NULL) { // Check for memory allocation failure
    perror("Memory allocation");
    exit(1);
  }

  // Check if the request method is "GET"
  if (!strcmp(request->method, "GET")) {
    // Adjust the request target (remove leading '/' for file path)
    request->request_target = request->request_target + 1;

    // Attempt to open the requested file
    FILE *file = fopen(request->request_target, "r");
    if (!file) { // If the file is not found
      // Set response headers to 404 Not Found and no file to send
      response->headers = "HTTP/1.1 404 Not Found\r\n"
                          "Content-Type: text/html\t\n\r\n";
      response->target_file = NULL;
    } else {
      // If the file exists, set response headers to 200 OK and send the file
      response->headers = "HTTP/1.1 200 OK\r\n"
                          "Content-Type: text/html\t\n\r\n";
      response->target_file = file;
    }
  } else { // If the method is not "GET"
    // Set response headers to 405 Method Not Allowed
    response->headers = "HTTP/1.1 405 Method Not Allowed\r\n"
                        "Content-Type: text/html\t\n\r\n";
    response->target_file = NULL; // No file to send
  }

  // TODO: Handle other HTTP methods such as HEAD, POST, PUT, TRACE...

  // Return the generated response
  return response;
}
