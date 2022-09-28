#include "resources.h"

#include <stdint.h>

static uint32_t s_num_meshes;
static struct mesh s_mesh_cache[MAX_CACHED_MESHES];

static uint32_t s_num_textures;
static struct texture s_texture_cache[MAX_CACHED_TEXTURES];

uint32_t load_mesh(const char *filepath) {
}
