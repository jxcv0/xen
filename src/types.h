#ifndef TYPES_H
#define TYPES_H

#include "maths.h"

// 3d mesh
typedef struct mesh_t
{
	void* mem_block;
	vec3_t* positions;
	vec3_t* normals;
	unsigned int* indices;
	size_t n_vertices;
	size_t n_indices;
	// vec3_t* tangents;
	// vec3_t* bitangents;
} mesh_t;

#endif // TYPES_H
