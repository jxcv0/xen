#include "resources.h"

#include <assimp/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include <assimp/cimport.h>
#include <assimp/defs.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3dmath.h"
#include "material.h"
#include "mesh.h"
#include "stb_image.h"
#include "texture.h"
#include "vertex.h"

#define MAX_ASSETS 64
#define MAX_FILEPATH_LEN 128

static struct map_entry {
  char m_filepath[MAX_FILEPATH_LEN];
  struct model *mp_model;
} s_map[64];

static int s_num_loaded = 0;

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
void load_textures(const char *dir, struct material *destmat,
           struct aiMaterial *material, enum aiTextureType type) {
  stbi_set_flip_vertically_on_load(true);
  int num_textures = aiGetMaterialTextureCount(material, type);
  for (int i = 0; i < num_textures; ++i) {
    // do the thing
  }
}

/*------------------------------------------------------------------------------
 */
void load_materials(const char *filepath, struct mesh *destmesh,
                   struct aiMaterial *material) {
  destmesh->mp_materials = 
  const char *last = strrchr(filepath, '/');
  size_t pathlen = strlen(filepath);
  size_t rm_len = strlen(last);
  size_t dirlen = pathlen - rm_len;
  char dir[dirlen + 1];
  strncpy(dir, filepath, dirlen);
  dir[dirlen] = '/';
  load_texures(dir, destmesh, material, aiTextureType_DIFFUSE);
}

/*------------------------------------------------------------------------------
 */
void process_mesh(const char *filepath, struct mesh *destmesh,
                  struct aiMesh *mesh, const struct aiScene *scene) {
  struct vertex *vertices = calloc(mesh->mNumVertices, sizeof(struct vertex));
  // assuming triangulated
  uint32_t *indices = calloc(mesh->mNumFaces * 3, sizeof(uint32_t));

  for (int i = 0; i < mesh->mNumVertices; ++i) {
    struct vertex *vertex = &vertices[i];
    vertex->m_position = construct_vec3(
        mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
    vertex->m_normal = construct_vec3(mesh->mNormals[i].x, mesh->mNormals[i].y,
                                      mesh->mNormals[i].z);
    vertex->m_tex_coord = construct_vec2(mesh->mTextureCoords[0][i].x,
                                         mesh->mTextureCoords[0][i].y);
    vertex->m_tangent = construct_vec3(
        mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
    vertex->m_bitangent = construct_vec3(
        mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
  }

  int idx = 0;
  for (int i = 0; i < mesh->mNumFaces; ++i) {
    struct aiFace face = mesh->mFaces[i];
    for (int j = 0; j < face.mNumIndices; ++j) {
      indices[idx] = (uint32_t)face.mIndices[j];
      ++idx;
    }
  }

  destmesh->m_num_vertices = mesh->mNumVertices;
  destmesh->mp_vertices = vertices;
  destmesh->m_num_indices = mesh->mNumFaces * 3;
  destmesh->mp_indices = indices;

  // we assume mesh only has one material for now
  struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  load_materials(filepath, destmesh, material);
}

/*------------------------------------------------------------------------------
 */
void process_node(const char *filepath, struct model *model,
                  struct aiNode *node, const struct aiScene *scene) {
  if (node != scene->mRootNode && node->mNumMeshes > 0) {
    // this is a child node
    model->mp_meshes =
        reallocarray(model->mp_meshes, model->m_num_meshes + node->mNumMeshes,
                     sizeof(struct mesh));
    if (NULL == model->mp_meshes) {
      perror("Unable to reallocate mesh memory");
      exit(EXIT_FAILURE);
    }
  } else if (scene->mNumMeshes > 0) {
    // this is the root node so allocate if we need to
    model->m_num_meshes = 0;
    model->mp_meshes = calloc(node->mNumMeshes, sizeof(struct mesh));
    if (NULL == model->mp_meshes) {
      perror("Unable to allocate mesh memory");
      exit(EXIT_FAILURE);
    }
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
  if (NULL == model) {
    perror("Unable to allocate model memory");
    return NULL;
  }

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
