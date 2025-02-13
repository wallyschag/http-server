#ifndef RESPONSE_GENERATOR_H
#define RESPONSE_GENERATOR_H

#include "../request-parser/request-parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

struct http_response {
  char *headers;
  FILE *target_file;
};

struct http_response *generate_response(struct http_request *request);

#endif // RESPONSE_GENERATOR_H
