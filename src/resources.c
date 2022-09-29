#include "resources.h"

#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t s_num_meshes = 0;
static struct mesh s_mesh_cache[MAX_CACHED_MESHES];

static uint32_t s_num_textures = 0;
static struct texture s_texture_cache[MAX_CACHED_TEXTURES];

/*------------------------------------------------------------------------------
 */
void process_node(const char *filepath, struct model *model,
                  struct aiNode *node, struct aiScene *scene) {}

/*------------------------------------------------------------------------------
 */
struct model resources_load_model(const char *filepath) {
  // if not already loaded ...

  const struct aiScene *scene = aiImportFile(
      filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices);
  if (NULL == scene) {
    perror(aiGetErrorString());
    exit(EXIT_FAILURE);
  }
  struct model model;

  process_node(filepath, &mesh, scene->mRoodNode, scene);
  aiReleaseImport(scene);
  return model;
}
