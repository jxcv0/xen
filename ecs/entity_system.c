#include "entity_system.h"
#include <assert.h>

static uint32_t signatures[MAX_ENTITIES] = {0};

static int free_list[MAX_ENTITIES];
static int front = 0;
static int back = 0;

/**
 * @brief Push an id to the back of the free list;
 *
 * @param id The id to push.
 * @return 0 on success, -1 on error.
 */
static int push_id(int id) {
  if (back != front) {
    free_list[back++] = id;
  } else {
    return -1;
  }
  if (back == MAX_ENTITIES) {
    back = 0;
  }
  return 0;
}

static int pop_entity(void) {
  if (front != back) {
    return free_list[front++];
  } else {
    return -1;
  }
  if (front == MAX_ENTITIES) {
    front = 0;
  }
  return -1;
}

void es_init(void) {
  for (int i = 0; i < MAX_ENTITIES; i++) {
    free_list[back++] = i;
  }
}

int es_create_entity(void) {
  return pop_entity();
}

void es_delete_entity(int id) {
  signatures[id] = 0;
  // broadcast deletion from here or elswhere??
  push_id(id);
}
