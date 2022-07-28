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

#include "graphics.h"

#include "shader.h"
#include "window.h"
#include "maths.h"

#include <stdlib.h>
#include <stdio.h>

// opengl debug output
void APIENTRY gl_debug_output(GLenum source,
			      GLenum type,
			      unsigned int id,
			      GLenum severity,
			      GLsizei length,
			      const char *message,
			      const void *userParam)
{
	printf("====================\n");
	printf("Debug message (%d): %s\n", id, message);
	switch(source)
	{
		case GL_DEBUG_SOURCE_API: printf("Source: API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: printf("Source: Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: printf("Source: Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION: printf("Source: Application"); break;
		case GL_DEBUG_SOURCE_OTHER: printf("Source: Other"); break;
	}
	printf("\n");

	switch (type)
	{
		case GL_DEBUG_TYPE_ERROR: printf("Type: Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: printf("Type: Undefined Behaviour"); break;
		case GL_DEBUG_TYPE_PORTABILITY: printf("Type: Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE: printf("Type: Performance"); break;
		case GL_DEBUG_TYPE_MARKER: printf("Type: Marker"); break;
		case GL_DEBUG_TYPE_PUSH_GROUP: printf("Type: Push Group"); break;
		case GL_DEBUG_TYPE_POP_GROUP: printf("Type: Pop Group"); break;
		case GL_DEBUG_TYPE_OTHER: printf("Type: Other"); break;
	}
	printf("\n");

	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH: printf("Severity: high"); break;
		case GL_DEBUG_SEVERITY_MEDIUM: printf("Severity: medium"); break;
		case GL_DEBUG_SEVERITY_LOW: printf("Severity: low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
	}
	printf("\n\n");
}

// init opengl settings
void graphics_init(void)
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

#ifdef XEN_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(gl_debug_output, NULL);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
	}
#endif
}

// check gl err
void checkerr_(const char *file, int line)
{
	GLenum errcode = GL_NO_ERROR;
	while ((errcode = glGetError()) != GL_NO_ERROR)
	{
		const char *error = NULL;
		switch (errcode)
		{
			case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		fprintf(stderr, "%s | %s (%d)\n", error, file, line);
		exit(EXIT_FAILURE);
	}
	return;
}

// generate gl buffers for mesh
int gen_buffer_objects(mesh_t *mesh)
{
	if (mesh->mem_block == NULL) { return -1; }
	glGenBuffers(1, &mesh->VBO);
	glGenVertexArrays(1, &mesh->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBindVertexArray(mesh->VAO);
	size_t vertices_size = mesh->num_vertices * sizeof(vec3_t);
	size_t texcoords_size = mesh->num_vertices * sizeof(vec2_t);
	size_t normals_size = mesh->num_vertices * sizeof(vec3_t);
	size_t VBO_size = vertices_size + texcoords_size + normals_size;
	glBufferData(GL_ARRAY_BUFFER, VBO_size, mesh->mem_block, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0); // vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (mesh->mem_block));
	glEnableVertexAttribArray(1); // texcoords
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (mesh->mem_block + vertices_size));
	glEnableVertexAttribArray(2); // normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (mesh->mem_block + vertices_size + texcoords_size));
	glBindVertexArray(0);
#ifdef XEN_DEBUG
	checkerr();
#endif
	return 0;
}

// update model matrix of a mesh
void update_model_matrix(mesh_t *mesh, vec3_t position, vec3_t rotation_axis, float angle)
{
	mat4_t mat = translate(construct_mat4(1.0f), position);
	mesh->model_matrix = rotate(mat, rotation_axis, radians(angle));
}

// draw a mesh
void draw_mesh(unsigned int shader, mesh_t* mesh)
{
	shader_use(shader);
	glBindVertexArray(mesh->VAO);
	glDrawArrays(GL_TRIANGLES, 0, mesh->num_vertices);
	glBindVertexArray(0);
}
