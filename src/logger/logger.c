#include <stdio.h>
#include <time.h>

#include "logger.h"

time_t now;
static struct tm *tm;

void log_message(char *message) {
  FILE *logfile;
  logfile = fopen("log.txt", "a");
  char log_message[128];

  now = time(0);
  if ((tm = localtime(&now)) == NULL) {
    perror("time");
    return;
  }

  snprintf(log_message, sizeof(log_message), "%d-%d-%d@%d:%d:%d -> %s\n",
           tm->tm_mon + 1, tm->tm_mday, tm->tm_year + 1900, tm->tm_hour + 1,
           tm->tm_min, tm->tm_sec, message);

  fputs(log_message, logfile);
  fclose(logfile);
}
