#ifndef MESH_H_
#define MESH_H_

#include <stdint.h>

#include "material.h"
#include "vertex.h"

/**
 * @brief Bundled mesh data.
 */
struct mesh {
  struct vertex *mp_vertices;
  uint32_t *mp_indices;
  struct material *mp_material;
  uint32_t m_num_vertices;
  uint32_t m_num_indices;
  uint32_t m_num_materials;
};

#endif // MESH_H_
