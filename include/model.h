#ifndef MODEL_H_
#define MODEL_H_

#include "mesh.h"

/**
 * @brief A model is a collection of meshes.
 */
struct model {
  struct mesh *mp_meshes;
  struct material *mp_materials;
};

#endif  // MODEL_H_
