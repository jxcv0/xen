#include <stdio.h>
#include "resources.h"

int main(int argc, char **argv) {
  (void) argc;
  (void) argv;

#ifndef L1DCLS
  printf("L1d cache line size unknown: using cache line size of 64 bytes.\n");
#else
  printf("Using L1d cache line size of ");
  printf("%d", L1DCLS);
  printf(" bytes.\n");
#endif

  printf("Hello, World!");
  return 0;
}
