#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#ifndef NDEBUG
#define log_debug(...) printf("DEBUG --> "); printf(__VA_ARGS__)
#else
#define log_debug(...)  // no-op
#endif

#define log_always(...) printf("INFO ---> "); printf(__VA_ARGS__)

#define log_fatal(...) printf("FATAL --> "); printf(__VA_ARGS__); \
  printf("*** errno: %s\n", strerror(errno)); exit(EXIT_FAILURE)

#endif  // LOGGER_H_
