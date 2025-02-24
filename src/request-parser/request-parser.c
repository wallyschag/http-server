#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request-parser.h"

// Helper function to turn a char * into a char ** based on newlines
char **parse_multiline_string(const char *input, int *count) {
  int size = 10; // Size of the char** array... How many char *s we can hold

  // Allocate our returnable char** array and error check.
  char **multiline_string = malloc(size * sizeof(char *));
  if (multiline_string == NULL) {
    perror("Memory allocation");
    exit(1);
  }

  // Set variables for tokenizing our strings by the '/n' (newline) delimiter
  char *token;
  char *delims = "\n";

  char *input_copy = strdup(input); // Duplicate our input string

  token = strtok(input_copy, delims); // Tokenize the copied input string.

  // While loop to continuously tokenize our string my the '\n' and store the
  // tokenized string in our char** array.
  while (token != NULL) {
    // Realloc if the ammount of lines is greater than the allocated size of the
    // char** array.
    if (*count == size) {
      size *= 2;
      char **temp = realloc(multiline_string, size * sizeof(char *));
      if (temp == NULL) {
        perror("Memory allocation");
        free(multiline_string);
        exit(1);
      }
      multiline_string = temp;
    }

    // Allocate and error check space in the char** array index for the incoming
    // line.
    multiline_string[*count] = malloc(strlen(token) + 1);
    if (multiline_string[*count] == NULL) {
      perror("Memory allocation");
      free(multiline_string);
      exit(1);
    }

    // Copy the token into the current index of the char** array.
    // Increment the number of lines and change the token to the
    // next token.
    strcpy(multiline_string[*count], token);
    (*count)++;
    token = strtok(NULL, delims);
  }

  return multiline_string;
}

char *find_content_type(char *filename) {
  if (filename == NULL) {
    return NULL;
  }

  char *content_type;
  char *ext = strrchr(filename, '.');

  if (strcmp(ext, ".css") == 0) {
    content_type = "text/css\t\n\r\n";
  } else if (strcmp(ext, ".gif") == 0) {
    content_type = "image/gif\t\n\r\n";
  } else if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
    content_type = "text/html\t\n\r\n";
  } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
    content_type = "image/jpeg\t\n\r\n";
  } else if (strcmp(ext, ".js") == 0) {
    content_type = "text/javascript\t\n\r\n";
  } else if (strcmp(ext, ".json") == 0) {
    content_type = "application/json\t\n\r\n";
  } else if (strcmp(ext, ".png") == 0) {
    content_type = "image/png\t\n\r\n";
  } else if (strcmp(ext, ".pdf") == 0) {
    content_type = "application/pdf\t\n\r\n";
  } else if (strcmp(ext, ".php") == 0) {
    content_type = "application/x-httpd-php\t\n\r\n";
  } else if (strcmp(ext, ".txt") == 0) {
    content_type = "text/plain\t\n\r\n";
  } else if (strcmp(ext, ".xml") == 0) {
    content_type = "application/xml\t\n\r\n";
  } else {
    content_type = "application/octet-stream\t\n\r\n";
  }

  return content_type;
}

// Parse the incoming request as a string and return an http_request struct
struct http_request *parse_request(char *receiving_buffer) {
  // Allocate and error check the http_request object to be returned
  struct http_request *request = malloc(sizeof(struct http_request));
  if (request == NULL) {
    perror("Memory allocation");
    exit(1);
  }

  // Set variables for tokenizing the first line of the string.
  int line_count = 0;
  char **headers = parse_multiline_string(receiving_buffer, &line_count);
  char *token;
  char *delims = " ";
  int token_count = 0;
  char *request_line = strdup(headers[0]);
  token = strtok(request_line, delims);

  // Loop to retreive the tokens from the first line of the string and put the
  // tokens in our http_request struct.
  while (token != NULL) {
    if (token_count == 0) {
      request->method = token;
    } else if (token_count == 1) {
      request->request_target = token;
      request->content_type = find_content_type(token);
    } else {
      request->protocol = token;
    }

    /*
     * TODO: Parse the remaining headers after the first line...
     *
     */

    token_count++;
    token = strtok(NULL, delims);
  }

  // Free the dynamically allocated char** array.
  // See parse_multiline_string()
  for (int i = 0; i < line_count; i++) {
    free(headers[i]);
  }
  free(headers);
  return request;
}
