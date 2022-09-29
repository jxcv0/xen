#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.h"

/**
 * @brief A model is a collection of meshes.
 */
struct model {
  uint32_t m_num_meshes;
  uint32_t *mp_meshes; // an array of indexes into the mesh cache.
};

#endif  // MODEL_H_
