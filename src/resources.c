#include "resources.h"
#include "logger.h"

#include <stddef.h>

/*------------------------------------------------------------------------------
 */
struct model *resources_load_obj(const char *filepath) {
  if (filepath == NULL) {
    log_always(WARN "model filepath is NULL\n");
    return NULL;
  }
  return NULL;
}
