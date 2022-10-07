#include "resources.h"
#include "logger.h"

#include <stddef.h>
#include <errno.h>
#include <assert.h>

/*------------------------------------------------------------------------------
 */
struct model *resources_load_obj(const char *filepath) {
  size_t pathlen = 0;
  if (filepath == NULL || (pathlen = strlen(filepath)) == 0) {
    log_always(WARN "filepath not usable: %s\n", filepath);
    return NULL;
  }

  log_debug(INFO "opening file: %s\n", filepath);
  int fd = fopen(filepath, O_RDONLY);
  if (fd == -1) {
    perror("unable to open file");
  }

  // get the size of the file and malloc buffer
  lseek(file, 0, SEEK_END);
  size_t fsize = ftell(file);
  char *bytes = malloc(fsize);
  lseek(file, 0, SEEK_SET);
  if (read(fd, bytes, fsize) == -1) { // read entire file into buffer
      perror("unable to read file");
      return NULL;
  }

  struct model *newmodel = malloc(sizeof(struct model));

  // do the model processing

  return newmodel;
}
