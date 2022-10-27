#include "entity_system.h"
#include <assert.h>

void tst_es_create_entity(void) {
  int e1 = es_create_entity();
  assert(e1 == 0);
  int e2 = es_create_entity();
  assert(e2 == 1);
  int e3 = es_create_entity();
  assert(e3 == 2);
}

int main(int, char**) {
  es_init();

  tst_es_create_entity();
  return 0;
}
