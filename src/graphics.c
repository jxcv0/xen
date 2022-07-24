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

#include "window.h"
#include "maths.h"

#include <stdlib.h>
#include <stdio.h>

static float scr_width = 800.0f;
static float scr_height = 800.0f;

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

// Init window with glfw
void graphics_init(void)
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* window = window_ptr();
	window = glfwCreateWindow(scr_width, scr_height, "TITLE", NULL, NULL);

	if (!window) {
		perror("Unable to create GLFW window\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, window_resize_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Unable to initialize GLAD\n");
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(window, camera_update_dir);

	glfwWindowHint(GLFW_SAMPLES, 4);
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

// shut down graphics system
void graphics_shutdown(void)
{
	glfwSetWindowShouldClose(window_ptr(), GL_TRUE);
	glfwTerminate();
}

// check if window should close
bool graphics_window_should_close(void)
{
	return glfwWindowShouldClose(window_ptr());
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
	}
	exit(EXIT_FAILURE);
	return;
}

// check compile status
void shader_check_compile(GLuint shader_id, const char* msg)
{
	GLint success;
	GLchar log[1024];
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
	glGetShaderInfoLog(shader_id, 1024, NULL, log);
	fprintf(stderr, "Shader program compilation error | %s\n%s\n", msg, log);
	}
}

// check link status
void shader_check_link(GLuint prgm_id)
{
	GLint success;
	GLchar log[1024];
	glGetProgramiv(prgm_id, GL_LINK_STATUS, &success);
	if (!success)
	{
	glGetProgramInfoLog(prgm_id, 1024, NULL, log);
	fprintf(stderr, "Shader program linking error | %s\n", log);
	}
}

// load, compile and link a shader from a file
unsigned int shader_load(const char* vert_path, const char* frag_path)
{
	FILE* file;
	char* vert_code;
	char* frag_code;

	// vert
	if ((file = fopen(vert_path, "r")) != NULL) {
		fseek(file, 0, SEEK_END);
		long f_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		vert_code = malloc(f_size);

		size_t n = 0;
		int c;

		while((c = fgetc(file)) != EOF)
		{
		    vert_code[n++] = (char)c;
		}
		vert_code[n] = '\0';
		fclose(file);
	} else {
		fprintf(stderr, "Unable to open vertex shader file | %s", vert_path);
		return -1;
	}

	// frag
	if ((file = fopen(frag_path, "r")) != NULL) {
		fseek(file, 0, SEEK_END);
		long f_size = ftell(file);
		fseek(file, 0, SEEK_SET);
		frag_code = malloc(f_size);

		size_t n = 0;
		int c;

		while((c = fgetc(file)) != EOF)
		{
		    frag_code[n++] = (char)c;
		}
		frag_code[n] = '\0';
		fclose(file);
	} else {
		fprintf(stderr, "Unable to open fragment shader file | %s", frag_path);
		return -1;
	}

	const char* const_vert_code = vert_code;
	unsigned int vert_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_id, 1, &const_vert_code, NULL); // wtf
	glCompileShader(vert_id);
	shader_check_compile(vert_id, vert_path);

	const char* const_frag_code = frag_code;
	unsigned int frag_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_id, 1, &const_frag_code, NULL);
	glCompileShader(frag_id);
	shader_check_compile(frag_id, frag_path);

	unsigned int program_id = glCreateProgram();
	glAttachShader(program_id, vert_id);
	glAttachShader(program_id, frag_id);
	glLinkProgram(program_id);
	shader_check_link(program_id);

	glDeleteShader(vert_id);
	glDeleteShader(frag_id);

	free(vert_code);
	free(frag_code);

	return program_id;
}
