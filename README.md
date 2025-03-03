# Simple HTTP Web Server in C

Welcome to my HTTP Server project! This project implements a basic HTTP server written in C that can receive HTTP requests from clients, parse them, generate a response, and send it back to the client. The server supports handling static file responses and dynamically parses incoming requests.

## WARNING
This project is not yet intended to be run on the open internet. Please keep all testing and usage to https://localhost:8080. This project contains many unresolved security vulnerabilities. Do not use this server application on the internet.

**YOU HAVE BEEN WARNED**

## Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)

## Introduction

This project is a simple HTTP server implemented in C. It accepts client connections, parses the incoming HTTP request, generates an HTTP response, and sends it back. The server can handle static file requests and return appropriate HTTP responses. It uses custom dynamic functions for request parsing and response generation. 

I created this project as a way to test and learn my security skills in code and network vulnerability.

## Features

- **Basic HTTP Request Parsing**: The server is capable of parsing the HTTP request sent by the client.
- **Dynamic Response Generation**: Based on the parsed request, the server generates the appropriate response, including sending files as part of the response body.
- **Multi-threading (future)**: Currently, the server is single-threaded, but it is designed in a way that can easily be extended to multi-threaded processing for handling multiple clients concurrently.
- **Socket Programming**: Uses raw socket programming to handle incoming requests and responses.

## Installation

To compile and run the server, follow these steps:
  1. Clone this repository
    `git clone https://github.com/wallyschag/http-server.git` 
  2. Navigate into the project directory:
    `cd http-server`
  3. Compile the project:
    `make`
  4. Run the server:
    `./http-server`

By default, the server will listen on port 8080. You can modify the port in the main.c file.

## Usage
Once the server is running, you can test it by making an HTTP request. You can use tools like `curl` or a web browser to interact with the server.

#### Example `curl` request:
`curl http://localhost:8080/`

## Contributing
Contributions are welcome! If you want to improve or add features to this project, feel free to fork the repository, create a branch, and submit a pull request.

© 2025 Wally Schageman
  
