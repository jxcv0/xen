// #define NDEBUG
#include "logger.h"

int main() {
  log_always(INFO "This message will appear even in NDEBUG\n");
  log_debug(DEBUG "This is a debug message\n");
  log_debug(INFO "10: %d TEN: %s\n", 10, "ten");
  int *var = NULL;
  if (var == NULL) {
    errno = ENOENT;
    log_fatal(WARN "var is null\n");
  }
  return 0;
}
