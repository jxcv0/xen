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

#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct io_request
{
	char filepath[64];
	void *buffer;
	size_t nbytes;
};

#define IO_REQUEST_BUFFER_SIZE 64
static struct io_request io_request_buffer[IO_REQUEST_BUFFER_SIZE];
static int io_index;
static pthread_t io_thread;
static pthread_mutex_t io_mutex;
static pthread_cond_t io_cond;
static int io_run;

// TODO 
// a way of querying the request buffer

static void* io_routine(void* arg)
{
	while(io_run)
	{
		pthread_mutex_lock(&io_mutex);
		while (io_index == 0)
		{
			pthread_cond_wait(&io_cond, &io_mutex);
		}
		struct io_request ior = io_request_buffer[--io_index];
		pthread_mutex_unlock(&io_mutex);
		pthread_cond_signal(&io_cond);
		int fd = open(ior.filepath, O_RDONLY);
		assert(fd != -1);
		ssize_t n_read = read(fd, ior.buffer, ior.nbytes);
		assert(n_read != -1);
		close(fd);
	}
	return NULL;
}

// initialize resource system
int io_init(void)
{
	io_run = 1;
	io_index = 0;
	return pthread_create(&io_thread, NULL, io_routine, NULL);
}

// shut down resource system
void io_shutdown(void)
{
	io_run = 0;
	io_index = 1; // this is dubious as [1] may be in use
	pthread_cond_signal(&io_cond);
	pthread_join(io_thread, NULL);
}

// request a file be loaded into a buffer
int io_request(const char *filepath, size_t pathlen, void *buffer, size_t nbytes)
{
	if (pathlen > 64) { return -1; }
	pthread_mutex_lock(&io_mutex);
	if (io_index == IO_REQUEST_BUFFER_SIZE - 1) {
		pthread_mutex_unlock(&io_mutex);
		pthread_cond_signal(&io_cond);
		return -1;
	}
	struct io_request *ior = &io_request_buffer[io_index++];
	strncpy(ior->filepath, filepath, pathlen);
	ior->buffer = buffer;
	ior->nbytes = nbytes;
	pthread_mutex_unlock(&io_mutex);
	pthread_cond_signal(&io_cond);
	return 0;
}

// wait for the request buffer to be empty
void io_wait(void)
{
	pthread_mutex_lock(&io_mutex);
	while (io_index != 0)
	{
		pthread_cond_wait(&io_cond, &io_mutex);
	}
	pthread_mutex_unlock(&io_mutex);
}

// load and parse *.obj file
// TODO materials (.mtl)
int io_load_mesh(mesh_t *mesh, const char* filepath)
{
	// open file
	FILE *file = NULL;
	if ((file = fopen(filepath, "r")) == NULL) {
		perror(filepath);
		return -1;
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
	vec3_t *vertices = calloc(v_count, sizeof(vec3_t));
	vec2_t *texcoords = calloc(vt_count, sizeof(vec2_t));
	vec3_t *normals = calloc(vn_count, sizeof(vec3_t));
	// allocate enough space for duplicating VTN sets
	mesh->num_vertices = f_count * 3; // 3 vertices per face
	size_t vertices_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t texcoords_size = sizeof(vec2_t) * mesh->num_vertices;
	size_t normals_size = sizeof(vec3_t) * mesh->num_vertices;
	size_t mem_block_size = vertices_size + texcoords_size + normals_size;
	mesh->mem_block = malloc(mem_block_size); // now with less mallocs!
	mesh->vertices = (vec3_t*)(mesh->mem_block);
	mesh->texcoords = (vec2_t*)(mesh->mem_block + vertices_size);
	mesh->normals = (vec3_t*)(mesh->mem_block + vertices_size + normals_size);
	v_count = 0;
	vt_count = 0;
	vn_count = 0;
	f_count = 0;
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
			for(;;)
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
				mesh->vertices[f_count] = vertices[index[0]];
				mesh->texcoords[f_count] = texcoords[index[1]];
				mesh->normals[f_count] = normals[index[2]];
				f_count++;
			}
		} else if (strncmp(token, "mtllib", 6) == 0) {
			// TODO handle materials
			continue;
		}
	}

	free(vertices);
	free(texcoords);
	free(normals);
	free(line);
	return 0;
}

void free_mesh(mesh_t *mesh)
{
	free(mesh->mem_block);
	mesh->mem_block = NULL;
	// other things ... 
}
