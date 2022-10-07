#include <stdio.h>
#include "resources.h"

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;
  
  struct model *model = resources_load_obj("assets/models/ucube/ucube.obj");
  return 0;
}
