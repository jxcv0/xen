#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <stdint.h>

/**
 * @brief A texture contains the image data and a hint for its intended use.
 */
struct texture {
  uint32_t m_width;
  uint32_t m_height;
  uint32_t m_num_channels;
  unsigned char *mp_data;
  enum texture_type { diffuse, specular, normal, height } m_type;
};

#endif  // TEXTURE_H_
