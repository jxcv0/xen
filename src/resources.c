#include "resources.h"
#include "logger.h"

#include <stddef.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

/*------------------------------------------------------------------------------
 */
struct model *resources_load_obj(const char *filepath) {
  size_t pathlen = 0;
  if (filepath == NULL || (pathlen = strlen(filepath)) == 0) {
    log_always(WARN "filepath not usable: %s\n", filepath);
    return NULL;
  }

  log_debug(INFO "opening file: %s\n", filepath);
  FILE *fstream = fopen(filepath, "r");
  if (fstream == NULL) {
    perror("unable to open file");
  }

  struct model *newmodel = malloc(sizeof(struct model));

  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  while ((nread = getline(&line, &len, fstream)) != -1) {
    printf("line: %s", line);
  }

  // do the model processing

  // return newmodel;
  return NULL;
}
