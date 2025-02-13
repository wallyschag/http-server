#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "request-parser.h"

/**
 * @brief Helper function that parses a multiline string into an array of
 * strings.
 *
 * This function takes a string containing multiple lines, splits it into
 * individual lines, and stores each line in a dynamically allocated array of
 * strings. The number of lines is also returned through the `count` parameter.
 *
 * @param input The multiline string to be parsed.
 * @param count Pointer to an integer that will store the number of lines after
 * parsing.
 * @return A dynamically allocated array of strings where each element is a line
 * from the input. The array needs to be freed after use.
 *
 * @note The function uses `strtok` to split the input string by newline
 * characters (`\n`). It dynamically resizes the array of strings as more lines
 * are found. The caller is responsible for freeing the memory allocated for the
 * array and its elements.
 */
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

/**
 * @brief Parses a http request and returns an http_request struct.
 *
 * This function takes a char* string that is separated into multiple lines.
 * First this function breaks the string into an array of strings using
 * parse_multiline_string() and then breaks down the first line of the string
 * into a http method (GET, POST, etc), the targeted resource, and the http
 * protocol version. It then saves these variables into a newly allocated
 * http_request struct, and then returns the struct.
 *
 * @param receiving_buffer The http request to be parsed.
 * @return A pointer to a http_request struct that contains the header values
 * parsed in the function.
 *
 * @note The function uses `strtok` to split the first line by space
 * characters (` `). The caller is responsible for freeing the dynamically
 * dynamically allocated http_request struct that is returned.
 *
 * @note Future work is required with regular expressions to save the rest of
 * the headers to the http_request struct. The rest of the headers are not
 * required for this early version but may be necessary later on.
 */
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
