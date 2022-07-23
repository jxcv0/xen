#ifndef TYPES_H
#define TYPES_H

#include "maths.h"

// 3d mesh no EBO
typedef struct mesh_t
{
	void* mem_block;
	vec3_t *vertices;
	vec3_t *normals;
	vec2_t *texcoords;
	size_t num_vertices;
	unsigned int VBO;
	unsigned int VAO;
	// material_t *materials;
	// TODO what are these again?:
	// vec3_t *tangents; 
	// vec3_t *bitangents;
} mesh_t;

#endif // TYPES_H
