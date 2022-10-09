#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>

#include "texture.h"

/**
 * @brief A material is a collection of textures.
 */
struct material {
  uint32_t m_num_textures;
  struct texture *mp_textures;
};

#endif // MATERIAL_H_
