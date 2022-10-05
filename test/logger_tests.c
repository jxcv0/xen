// #define NDEBUG
#include "logger.h"

int main() {
    log_always("This message will appear even in NDEBUG\n");
    log_debug("This is a debug message\n");
    log_debug("10: %d TEN: %s\n", 10, "ten");
    int *var = NULL;
    if (var == NULL) {
        errno = ENOENT;
        log_fatal("var is null\n");
    }
    return 0;
}
