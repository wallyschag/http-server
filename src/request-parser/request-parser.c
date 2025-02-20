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

  if (strcmp(ext, ".aac") == 0) {
    content_type = "audio/aac";
  } else if (strcmp(ext, ".abw") == 0) {
    content_type = "application/x-abiword";
  } else if (strcmp(ext, ".apng") == 0) {
    content_type = "image/apng";
  } else if (strcmp(ext, ".arc") == 0) {
    content_type = "application/x-freearc";
  } else if (strcmp(ext, ".avif") == 0) {
    content_type = "image/avif";
  } else if (strcmp(ext, ".avi") == 0) {
    content_type = "video/x-msvideo";
  } else if (strcmp(ext, ".azw") == 0) {
    content_type = "application/vnd.amazon.ebook";
  } else if (strcmp(ext, ".bin") == 0) {
    content_type = "application/octet-stream";
  } else if (strcmp(ext, ".bmp") == 0) {
    content_type = "image/bmp";
  } else if (strcmp(ext, ".bz") == 0) {
    content_type = "application/x-bzip";
  } else if (strcmp(ext, ".bz2") == 0) {
    content_type = "application/x-bzip2";
  } else if (strcmp(ext, ".cda") == 0) {
    content_type = "application/x-cdf";
  } else if (strcmp(ext, ".csh") == 0) {
    content_type = "application/x-csh";
  } else if (strcmp(ext, ".css") == 0) {
    content_type = "text/css";
  } else if (strcmp(ext, ".csv") == 0) {
    content_type = "text/csv";
  } else if (strcmp(ext, ".doc") == 0) {
    content_type = "application/msword";
  } else if (strcmp(ext, ".docx") == 0) {
    content_type =
        "application/"
        "vnd.openxmlformats-officedocument.wordprocessingml.document";
  } else if (strcmp(ext, ".eot") == 0) {
    content_type = "application/vnd.ms-fontobject";
  } else if (strcmp(ext, ".epub") == 0) {
    content_type = "application/epub+zip";
  } else if (strcmp(ext, ".gz") == 0) {
    content_type = "application/gzip";
  } else if (strcmp(ext, ".gif") == 0) {
    content_type = "image/gif";
  } else if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0) {
    content_type = "text/html";
  } else if (strcmp(ext, ".ico") == 0) {
    content_type = "image/vnd.microsoft.icon";
  } else if (strcmp(ext, ".ics") == 0) {
    content_type = "text/calendar";
  } else if (strcmp(ext, ".jar") == 0) {
    content_type = "application/java-archive";
  } else if (strcmp(ext, ".jpeg") == 0 || strcmp(ext, "jpg") == 0) {
    content_type = "image/jpeg";
  } else if (strcmp(ext, ".js") == 0) {
    content_type = "text/javascript";
  } else if (strcmp(ext, ".json") == 0) {
    content_type = "application/json";
  } else if (strcmp(ext, ".jsonld") == 0) {
    content_type = "application/ld+json";
  } else if (strcmp(ext, ".mid") == 0 || strcmp(ext, ".midi") == 0) {
    content_type = "audio/midi";
  } else if (strcmp(ext, ".mp3") == 0) {
    content_type = "audio/mpeg";
  } else if (strcmp(ext, ".mp4") == 0) {
    content_type = "video/mp4";
  } else if (strcmp(ext, ".mpeg") == 0) {
    content_type = "video/mpeg";
  } else if (strcmp(ext, ".mpkg") == 0) {
    content_type = "application/vnd.apple.installer+xml";
  } else if (strcmp(ext, ".odp") == 0) {
    content_type = "application/vnd.oasis.opendocument.presentation";
  } else if (strcmp(ext, ".ods") == 0) {
    content_type = "application/vnd.oasis.opendocument.spreadsheet";
  } else if (strcmp(ext, ".odt") == 0) {
    content_type = "application/vnd.oasis.opendocument.text";
  } else if (strcmp(ext, ".oga") == 0) {
    content_type = "audio/ogg";
  } else if (strcmp(ext, ".ogx") == 0) {
    content_type = "application/ogg";
  } else if (strcmp(ext, ".opus") == 0) {
    content_type = "audio/ogg";
  } else if (strcmp(ext, ".otf") == 0) {
    content_type = "font/otf";
  } else if (strcmp(ext, ".png") == 0) {
    content_type = "image/png";
  } else if (strcmp(ext, ".pdf") == 0) {
    content_type = "application/pdf";
  } else if (strcmp(ext, ".php") == 0) {
    content_type = "application/x-httpd-php";
  } else if (strcmp(ext, ".ppt") == 0) {
    content_type = "application/vnd.ms-powerpoint";
  } else if (strcmp(ext, ".pptx") == 0) {
    content_type =
        "application/"
        "vnd.openxmlformats-officedocument.presentationml.presentation";
  } else if (strcmp(ext, ".rar") == 0) {
    content_type = "application/vnd.rar";
  } else if (strcmp(ext, ".rtf") == 0) {
    content_type = "application/rtf";
  } else if (strcmp(ext, ".sh") == 0) {
    content_type = "application/x-sh";
  } else if (strcmp(ext, ".svg") == 0) {
    content_type = "image/svg+xml";
  } else if (strcmp(ext, ".tar") == 0) {
    content_type = "application/x-tar";
  } else if (strcmp(ext, ".tif") == 0 || strcmp(ext, ".tiff")) {
    content_type = "image/tiff";
  } else if (strcmp(ext, ".ts") == 0) {
    content_type = "video/mp2t";
  } else if (strcmp(ext, ".ttf") == 0) {
    content_type = "font/ttf";
  } else if (strcmp(ext, ".txt") == 0) {
    content_type = "text/plain";
  } else if (strcmp(ext, ".vsd") == 0) {
    content_type = "application/vnd.visio";
  } else if (strcmp(ext, ".wav") == 0) {
    content_type = "audio/wav";
  } else if (strcmp(ext, ".weba") == 0) {
    content_type = "audio/webm";
  } else if (strcmp(ext, ".webm") == 0) {
    content_type = "video/webm";
  } else if (strcmp(ext, ".webp") == 0) {
    content_type = "image/webp";
  } else if (strcmp(ext, ".woff") == 0) {
    content_type = "font/woff";
  } else if (strcmp(ext, ".woff2") == 0) {
    content_type = "font/woff2";
  } else if (strcmp(ext, ".xhtml") == 0) {
    content_type = "application/xhtml+xml";
  } else if (strcmp(ext, ".xls") == 0) {
    content_type = "application/vnd.ms-excel";
  } else if (strcmp(ext, ".xlsx") == 0) {
    content_type =
        "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
  } else if (strcmp(ext, ".xml") == 0) {
    content_type = "application/xml";
  } else if (strcmp(ext, ".xul") == 0) {
    content_type = "application/vnd.mozilla.xul+xml";
  } else if (strcmp(ext, ".zip") == 0) {
    content_type = "application/zip";
  } else if (strcmp(ext, ".3gp") == 0) {
    content_type = "video/3gpp";
  } else if (strcmp(ext, ".3g2") == 0) {
    content_type = "video/3gpp2";
  } else if (strcmp(ext, ".7z") == 0) {
    content_type = "application/x-7z-compressed";
  } else {
    content_type = "application/octet-stream";
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
      // TODO: Set the content-type here
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
