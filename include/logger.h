#ifndef LOGGER_H_
#define LOGGER_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define get_filename()                                                         \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define INFO "info: "
#define WARN "warning: "
#define DEBUG "debug: "
#define ERROR "error: "

#ifndef NDEBUG
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...) // no-op
#endif

#define log_always(...) printf(__VA_ARGS__)

#define log_fatal(...)                                                         \
  printf(__VA_ARGS__);                                                         \
  exit(EXIT_FAILURE)

#endif // LOGGER_H_
