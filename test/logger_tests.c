#include "logger.h"

int main() {
    log_debug("This is a debug message\n");
    log_debug("10: %d TEN: %s\n", 10, "ten");
    return 0;
}
