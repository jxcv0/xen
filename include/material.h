#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "texture.h"

/**
 * @brief A material is a collection of textures.
 */
struct material {
  struct texture *mp_textures;
};

#endif  // MATERIAL_H_
