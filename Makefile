# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -D_POSIX_C_SOURCE=200809L

# Directories
SRC_DIR = src
REQUEST_PARSER_DIR = $(SRC_DIR)/request-parser
RESPONSE_GENERATOR_DIR = $(SRC_DIR)/response-generator
SERVER_PROCESSING_DIR = $(SRC_DIR)/server-processing
LOGGER_DIR = $(SRC_DIR)/logger

# Source files
SRC_FILES = $(SRC_DIR)/main.c \
            $(REQUEST_PARSER_DIR)/request-parser.c \
            $(RESPONSE_GENERATOR_DIR)/response-generator.c \
						$(SERVER_PROCESSING_DIR)/server-processing.c \
						$(LOGGER_DIR)/logger.c

# Header files
HEADERS = $(REQUEST_PARSER_DIR)/request-parser.h \
          $(RESPONSE_GENERATOR_DIR)/response-generator.h \
					$(SERVER_PROCESSING_DIR)/server-processing.h \
					$(LOGGER_DIR)/logger.h

# Output binary
OUTPUT = http-server

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Default target
all: $(OUTPUT)

# Link the object files to create the executable
$(OUTPUT): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $(OUTPUT)

# Rule to compile .c files to .o files
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean the project (remove generated files)
clean:
	rm -f $(OBJ_FILES) $(OUTPUT)

# Rebuild everything
rebuild: clean all

# Install the server (optional)
install: $(OUTPUT)
	# Example: cp $(OUTPUT) /usr/local/bin/

# Phony targets
.PHONY: all clean rebuild install
