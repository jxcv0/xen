#ifndef TYPES_H
#define TYPES_H

#include "maths.h"

typedef struct material
{
	// the things
} material_t;

// 3d mesh no EBO
typedef struct mesh
{
	void* mem_block;
	vec3_t *vertices;
	vec2_t *texcoords;
	vec3_t *normals;
	size_t num_vertices;
	unsigned int VBO;
	unsigned int VAO;
	// material_t *materials;
	// TODO what are these again?:
} mesh_t;

typedef struct light
{
    int id;
    vec3_t color;
    vec3_t position;
    float constant;
    float linear;
    float quadratic;
} light_t;

#endif // TYPES_H
