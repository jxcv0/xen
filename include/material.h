#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>

/**
 * @brief A material is a collection of textures.
 */
struct material
{
  uint32_t *mp_textures;
};

#endif // MATERIAL_H_
