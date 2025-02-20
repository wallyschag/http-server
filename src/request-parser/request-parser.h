// request-parser.h
#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct http_request {
  char *method;
  char *request_target;
  char *protocol;
  char *content_type;
  // char *user_agent; // Future implementation
  // int content_length; // Future implementation
};

char **parse_multiline_string(const char *input, int *count);
struct http_request *parse_request(char *receiving_buffer);

#endif // !REQUEST_PARSER_H
