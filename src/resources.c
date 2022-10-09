#include "resources.h"
#include "logger.h"

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define NEEDLE_VERTEX  "v "
#define NEEDLE_OBJECT  "o "
#define NEEDLE_FACE    "f "
#define NEEDLE_PARAM   "vp "
#define NEEDLE_LINE    "l "
#define NEEDLE_NORMAL  "vn "
#define NEEDLE_TEXTURE "vt "
#define NEEDLE_USEMTL  "usemtl "
#define NEEDLE_MTLLIB  "mtllib "
#define NEEDLE_SMOOTH  "s "

/*------------------------------------------------------------------------------
 */
enum line_type {
  object,
  vertex,
  face,
  param,
  line,
  normal,
  texture,
  usemtl,
  mtllib,
  smooth
};

/*------------------------------------------------------------------------------
 */
struct line {
  enum line_type m_type;
  char *mp_line;
};

/*------------------------------------------------------------------------------
 */
static struct line construct_line(const char *line) {
  struct line l = {};
  // do the things
  return l;
}

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
