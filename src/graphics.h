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

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>

#include "types.h"

// init opengl settings
void graphics_init(void);

// shut down graphics system
void graphics_shutdown(void);

// check gl err
void checkerr_(const char *file, int line);
#define checkerr() checkerr_(__FILE__, __LINE__)

// create VAO and VBO
void update_model_matrix(mesh_t*, vec3_t, vec3_t, float);

// generate gl buffers for mesh
int gen_buffer_objects(mesh_t*);

// draw a mesh
void draw_mesh(unsigned int, mesh_t*);

#endif // GRAPHICS_H
