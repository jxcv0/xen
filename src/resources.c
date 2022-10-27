#include "resources.h"
#include "logger.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_RESOURCES 512

static struct {
    char m_filepath[MAX_PATH_LEN];
    void* mp_resource;
} s_loaded_paths[MAX_RESOURCES];

/*------------------------------------------------------------------------------
 */
int get_loaded_resource(char filepath[MAX_PATH_LEN]) {
  for (int i = 0; i < MAX_RESOURCES; i++) {
    if (strncmp(s_loaded_paths[i].m_filepath, filepath, MAX_PATH_LEN) == 0) {
      return i;
    }
  }
  return -1;
}

/*------------------------------------------------------------------------------
 */
struct model *resources_load_obj(char filepath[MAX_PATH_LEN]) {
  size_t pathlen = 0;
  if (filepath == NULL || (pathlen = strlen(filepath)) == 0) {
    log_always(WARN "filepath not usable: %s\n", filepath);
    return NULL;
  }

  int idx = -1;
  if ((idx = get_loaded_resource(filepath)) != -1) {
    return (struct model*)s_loaded_paths[idx].mp_resource;
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
