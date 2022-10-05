#ifndef LOGGER_H_
#define LOGGER_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFO  "INFO --->"
#define WARN  "WARN --->"
#define DEBUG "DEBUG -->"
#define ERROR "ERROR -->"

#ifndef NDEBUG
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...)  // no-op
#endif

#define log_always(...) printf(__VA_ARGS__)

#define log_fatal(...) printf(__VA_ARGS__); exit(EXIT_FAILURE)

#endif  // LOGGER_H_
