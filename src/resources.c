/*
 * Copyright © 2022 Jacob Micoud
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "resources.h"

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// load and parse *.obj file
// TODO materials (.mtl)
static void* io_load_mesh(void* arg)
{
	struct io_request *ior = (struct io_request*)arg;
	mesh_t *mesh = (mesh_t*)ior->struct_ptr;
	const char *filepath = ior->filepath;

	// open file
	FILE *file = NULL;
	if ((file = fopen(filepath, "r")) == NULL) {
		perror(filepath);
		mesh = NULL;
		return NULL;
	}

	int v_count = 0, vt_count = 0, vn_count = 0, f_count = 0;
	char *line = NULL;
	size_t len = 0;
	ssize_t nread = 0;
	while ((nread = getline(&line, &len, file)) != -1) // count the number of vectors
	{
		if (strstr(line, "v ") != NULL) {
			v_count++;
		} else if (strstr(line, "vt ") != NULL) {
			vt_count++;
		} else if (strstr(line, "vn ") != NULL) {
			vn_count++;
		} else if (strstr(line, "f ") != NULL) {
			f_count++;
		}
	}

	// temp storage
	vec3_t vertices[v_count];
	vec2_t texcoords[vt_count];
	vec3_t normals[vn_count];
	// allocate enough space for duplicating VTN sets
	mesh->num_vertices = f_count * 3; // 3 vertices per face
	size_t vertices_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t texcoords_size = sizeof(vec2_t) * mesh->num_vertices;
	size_t normals_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t mem_block_size = vertices_size + texcoords_size + normals_size;
	mesh->mem_block = malloc(mem_block_size);
	if (mesh->mem_block == NULL) {
		mesh = NULL;
		return NULL;
	}
	mesh->vertices = (vec3_t*)(mesh->mem_block);
	mesh->texcoords = (vec2_t*)(mesh->mem_block + vertices_size);
	mesh->normals = (vec3_t*)(mesh->mem_block + vertices_size + normals_size);
	v_count = 0;
	vt_count = 0;
	vn_count = 0;
	int it = 0;
	rewind(file);

	while ((nread = getline(&line, &len, file)) != -1)
	{
		char *linesave = NULL;
		char *token = strtok_r(line, " ", &linesave);
		if (strncmp(token, "#", 2) == 0) { // comments
			continue;
		} else if (strncmp(token, "mtllib", 6) == 0) { // material library
			// TODO handle materials
			continue;
		} else if (strncmp(token, "o", 2) == 0) { // object name
			continue;
		} else if (strncmp(token, "v", 2) == 0) { // vertices
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				vertices[v_count].values[i] = strtof(token, NULL);
			}
			v_count++;
		} else if (strncmp(token, "vt", 2) == 0) { // texcoords
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				texcoords[vt_count].values[i] = strtof(token, NULL);
			}
			vt_count++;
		} else if (strncmp(token, "vn", 2) == 0) { // normals
			for(int i = 0; ; i++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				normals[vn_count].values[i] = strtof(token, NULL);
			}
			vn_count++;
		} else if (strncmp(token, "s", 2) == 0) { // smooth shading always off
			continue;
		} else if (strncmp(token, "f", 2) == 0) { // faces
			char *toksave = NULL;
			for(; it < f_count; it++)
			{
				token = strtok_r(NULL, " ", &linesave);
				if (token == NULL) { break; }
				int  i = 0;
				int index[3];
				for (char* tok = token; ; i++, tok = NULL)
				{
					char* subtok = strtok_r(tok, "/", &toksave);
					if (subtok == NULL) { break; }
					index[i] = atoi(subtok) - 1;
				}
				mesh->vertices[it] = vertices[index[0]];
				mesh->texcoords[it] = texcoords[index[1]];
				mesh->normals[it] = normals[index[2]];
			}
		} else if (strncmp(token, "mtllib", 6) == 0) {
			// TODO handle materials
			continue;
		}
	}
	free(line);
	fclose(file);
	pthread_exit(NULL);
	return NULL;
}

// create a new thread to parse an *.obj file into a mesh_t
int io_load_mesh_async(struct io_request *ior)
{
	return pthread_create(ior->thread_ptr, NULL, io_load_mesh, ior);
}

// free mesh memory
void free_mesh(mesh_t *mesh)
{
	free(mesh->mem_block);
	mesh->mem_block = NULL;
	// other things ... 
}

// load shader frag/vert text from file
// struct_ptr is cast to char* to contain the text GLSL
// freeing this memory must be done in calling function
static void* io_load_text(void* arg)
{
	struct io_request *ior = (struct io_request*)arg;
	char* text = (char*)ior->struct_ptr;
	FILE *file = NULL;
	if ((file = fopen(ior->filepath, "r")) == NULL) {
		ior->struct_ptr = NULL;
		fprintf(stderr, "Unable to open file | %s", ior->filepath);
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	text = malloc(file_size);
	size_t n = 0;
	int c;
	while ((c = fgetc(file)) != EOF)
	{
		text[n++] = (char)c;
	}
	text[n] = '\0';
	fclose(file);
	pthread_exit(NULL);
	return NULL;
}

int io_load_text_async(struct io_request *ior)
{
	return pthread_create(ior->thread_ptr, NULL, io_load_text, ior);
}
