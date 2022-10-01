#include "resources.h"

#include "material.h"
#include "mesh.h"
#include "texture.h"
#include "vertex.h"
#include <assimp/cimport.h>
#include <assimp/defs.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ASSETS 64
#define MAX_FILEPATH_LEN 128
static struct map_entry {
  char m_filepath[MAX_FILEPATH_LEN];
  struct model *mp_model;
} s_map[64];

static int s_num_loaded;

/**-----------------------------------------------------------------------------
 * @brief Check if a Model has been loaded from a filepath already.
 *
 * @param filepath The filpath of the model.
 * @return The pointer to the model if it is already allocated
 */
struct model *is_loaded(const char *filepath) {
  for (int i = 0; i < s_num_loaded; ++i) {
    struct map_entry *entry = &s_map[i];
    if (strcmp(entry->m_filepath, filepath) == 0) {
      return entry->mp_model;
    }
  }
  return NULL;
}

/*------------------------------------------------------------------------------
 */
void process_mesh(const char *filepath, struct mesh *destmesh, aiMesh *mesh,
                  const struct aiScene *scene) {
  struct vertex *vertices = calloc(mesh->mNumVertices, sizeof(struct vertex));
  uint32_t indices = calloc(mesh->mNumFaces * 3, sizeof(uint32_t));
  
  // ...

}

/*------------------------------------------------------------------------------
 */
void process_node(const char *filepath, struct model *model,
                  struct aiNode *node, const struct aiScene *scene) {

  if (model->m_num_meshes > 0) {
    // if this is a child node
    model->mp_meshes =
        reallocarray(model->mp_meshes, model->m_num_meshes + node->mNumMeshes,
                     sizeof(struct mesh));
  } else {
    // if this is the root node
    model->m_num_meshes = 0;
    model->mp_meshes = calloc(node->mNumMeshes, sizeof(struct mesh));
  }

  // TODO an allocator to sensibly store meshes
  for (int i = 0; i < node->mNumMeshes; ++i) {
    struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    process_mesh(filepath, &model->mp_meshes[model->m_num_meshes], mesh, scene);
    ++model->m_num_meshes;
  }

  for (int i = 0; i < node->mNumChildren; ++i) {
    process_node(filepath, model, node->mChildren[i], scene);
  }
}

/*------------------------------------------------------------------------------
 */
struct model *resources_load_model(const char *filepath) {
  struct model *model = is_loaded(filepath);
  if (NULL != model) {
    return model;
  }

  // TODO an allocator that sensibly stores models
  model = malloc(sizeof(struct model));

  const struct aiScene *scene = aiImportFile(
      filepath, aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                    aiProcess_JoinIdenticalVertices);
  if (NULL == scene) {
    perror(aiGetErrorString());
    exit(EXIT_FAILURE);
  }

  process_node(filepath, model, scene->mRootNode, scene);
  s_map[s_num_loaded].mp_model = model;
  strncpy(s_map[s_num_loaded].m_filepath, filepath, MAX_FILEPATH_LEN);
  ++s_num_loaded;
  aiReleaseImport(scene);
  return model;
}
