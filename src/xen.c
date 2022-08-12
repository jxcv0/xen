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

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#include "types.h"

// window
static GLFWwindow *WINDOW;
static float scr_width = 800.0f;
static float scr_height = 600.0f;

void xen_init(void);
void on_resize(GLFWwindow*, int, int);
void frame_start(void);
void frame_end(void);
int load_mesh(const char*);

int main(void)
{
	xen_init();

	while (!glfwWindowShouldClose(WINDOW))
	{
		frame_start();
		if (glfwGetKey(WINDOW, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetWindowShouldClose(WINDOW, GLFW_TRUE);
		}
		frame_end();
	}

	glfwTerminate();
	return 0;
}

// TODO opengl version detection
void xen_init(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	WINDOW = glfwCreateWindow(scr_width, scr_height, "XEN", NULL, NULL);

	if (!WINDOW) {
		perror("Unable to create GLFW window\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(WINDOW);
	glfwSetFramebufferSizeCallback(WINDOW, on_resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Unable to initialize GLAD\n");
	}

	glfwSetInputMode(WINDOW, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void on_resize(GLFWwindow* window, int w, int h)
{
	if (window) {
		glViewport(0, 0, w, h);
	}
}

void frame_start(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void frame_end(void)
{
	glfwSwapBuffers(WINDOW);
	glfwPollEvents();
}
