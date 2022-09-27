#ifndef TEXTURE_H_
#define TEXTURE_H_

/**
 * @brief A texture contains the image data and a hint for its intended use.
 */
struct texture {
  unsigned char *mp_data;
  enum texture_type { diffuse, specular, normal, height } m_type;
};

#endif  // TEXTURE_H_
