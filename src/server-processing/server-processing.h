// server-processing.h
#ifndef SERVER_PROCESSING_H
#define SERVER_PROCESSING_H

int accept_connection(int serverfd);
int process_connection(int clientfd);

#endif // !SERVER_PROCESSING_H
