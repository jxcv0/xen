#ifndef TYPES_H
#define TYPES_H

#include "maths.h"

// 3d mesh
typedef struct mesh_t
{
	void* mem_block;
	vec3_t *vertices;
	vec3_t *normals;
	vec2_t *texcoords;
	unsigned int *indices;
	size_t num_vertices;
	size_t num_indices;
	// TODO what are these again?:
	// vec3_t *tangents; 
	// vec3_t *bitangents;
} mesh_t;

#endif // TYPES_H
