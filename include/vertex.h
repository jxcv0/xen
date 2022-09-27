#ifndef VERTEX_H_
#define VERTEX_H_

#include "3dmath.h"

struct vertex {
  vec3_t m_position;
  vec3_t m_normal;
  vec2_t m_tex_coord;
  vec3_t m_tangent;
  vec3_t m_bitangent;
};

#endif  // VERTEX_H_
