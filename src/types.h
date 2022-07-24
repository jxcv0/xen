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
