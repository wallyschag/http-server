#include <stdarg.h>
#include <stdio.h>
#include <time.h>

#include "logger.h"

time_t now;
static struct tm *tm;

void log_message(char *message, ...) {
  va_list args;            // Arguments passed into ...
  va_start(args, message); // Initialize va list
  char format_message[1024];
  vsnprintf(format_message, sizeof(format_message), message,
            args); // Format the message and store it in format_message buffer

  // Open log file for appending
  FILE *logfile;
  logfile = fopen("log.txt", "a");

  // Create buffer to store the final log message
  char log_message[2048];

  // Get current time
  now = time(0);
  if ((tm = localtime(&now)) == NULL) {
    perror("time");
    return;
  }

  // Format message with the current time
  snprintf(log_message, sizeof(log_message), "%d-%d-%d@%d:%d:%d -> %s\n",
           tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900, tm->tm_hour + 1,
           tm->tm_min, tm->tm_sec, format_message);
  fputs(log_message, logfile); // Put the formatted message in the log file
  fclose(logfile);             // Close the log file

  va_end(args); // Cleanup argument list
}
