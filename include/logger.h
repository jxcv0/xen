#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>

#ifndef NDEBUG
#define log_debug(...) printf(__VA_ARGS__)
#else
#define log_debug(...)  // no-op
#endif

#endif  // LOGGER_H_
