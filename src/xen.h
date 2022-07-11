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

#ifndef XEN_H
#define XEN_H

#include "maths.h"
#include "input_sys.h"

#include "glad.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>

// global window for static inlines
extern GLFWwindow* window;

// check for gl errs
void checkerror_(const char *file, int line);
#define checkerr() checkerror_(__FILE__, __LINE__)

// opengl debug callback
void APIENTRY gl_debug_output(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char *message,
                              const void *userParam);

typedef struct light_t
{
    int id;
    vec3_t color;
    vec3_t position;
    float constant;
    float linear;
    float quadratic;
} light_t;

// create a light
light_t create_default_light();

// check compile status
void shader_check_compile(GLuint shader_id, const char* msg);

// check link status
void shader_check_link(GLuint prgm_id);

// load and compile a shader from a text file
unsigned int shader_load(const char* vert_path, const char* frag_path);

// set a shader as the active shader
static inline void shader_use(unsigned int shader)
{
    glUseProgram(shader);
}

// lazy overloads
#define shader_set_uniform(x, y, z) _Generic((z), \
    int: shader_set_uniform_int, \
    float: shader_set_uniform_float, \
    vec2_t: shader_set_uniform_vec2, \
    vec3_t: shader_set_uniform_vec3, \
    mat4_t: shader_set_uniform_mat4, \
    light_t*: shader_set_uniform_light \
    )(x, y, z)

// set shader uniform utility function
void shader_set_uniform_light(unsigned int shader,
                              unsigned int light_index,
                              const light_t* light);

// set shader uniform utility function
static inline void shader_set_uniform_int(unsigned int shader, const char* uniform_name, const int i)
{
    glUniform1i(glGetUniformLocation(shader, uniform_name), i);
}

// set shader uniform utility function
static inline void shader_set_uniform_float(unsigned int shader, const char* uniform_name, const float f)
{
    glUniform1f(glGetUniformLocation(shader, uniform_name), f);
}

// set shader uniform utility function
static inline void shader_set_uniform_vec2(unsigned int shader, const char* uniform_name, const vec2_t v)
{
    glUniform2fv(glGetUniformLocation(shader, uniform_name), 1, &v.values[0]);
}

// set shader uniform utility function
static inline void shader_set_uniform_vec3(unsigned int shader, const char* uniform_name, const vec3_t v)
{
    glUniform3fv(glGetUniformLocation(shader, uniform_name), 1, &v.values[0]);
}

// set shader uniform utility function
static inline void shader_set_uniform_mat4(unsigned int shader, const char* uniform_name, const mat4_t m)
{
    glUniformMatrix4fv(glGetUniformLocation(shader, uniform_name), 1, GL_FALSE, &m.values[0][0]);
}

typedef struct mesh_t
{
    void* mem_block;
    vec3_t* positions;
    vec3_t* normals;
    vec2_t* tex_coords;
    vec3_t* tangents;
    vec3_t* bitangents;
    int* indices;

    unsigned int VAO, VBO, EBO;
    size_t n_vertices;
    size_t n_indices;

    unsigned int tex_ids[3];
    
    vec3_t world_position;
    float rot_b;
} mesh_t;

// load a texure from a file and add data to gl buffers
unsigned int load_texture(const char *dir, const char* tex_name);

// for now we assume that each model has 3 texture maps in the same dir, the file is .obj and all 3
// textures are .png
int mesh_load(mesh_t* mesh, const char *dir, const char* name);

// load a mesh with no texture data
int mesh_load_simple(mesh_t *mesh, const char* filepath);

// free mesh memory
void free_mesh(mesh_t *mesh);

// draw a simple mesh with no textures
void draw_mesh_simple(mesh_t *mesh, unsigned int shader);

// draw a mesh
void draw_mesh(mesh_t *mesh, unsigned int shader);

// initialize the camera direction based on screen size and cursor position
void camera_dir_init();

// update the camera direction based on a change in mouse position
// default version rotates about world origin
void camera_update_dir(GLFWwindow *window, double x, double y);

// update the camera direction based on a change in mouse position
// debug version rotates about camera_pos
void camera_update_dir_debug(GLFWwindow *window, double x, double y);

// get the current camera position
vec3_t get_camera_pos();

// create a model matrix from a mesh position and rotation
mat4_t mesh_model_matrix(const mesh_t *mesh);

// generate a view matrix from the camera
mat4_t camera_view_matrix(void);

// window resize callback
void on_resize(GLFWwindow *window, int width, int height);

// initialize window and opengl
void xen_init();

// get the aspect ratio of the screen
float get_aspect();

// get input from glfw window
void handle_input(mesh_t *mesh, float delta_t);

void frame_start(); // get input, ...
void frame_end(); // wait for jobs, clear input buffer, ...

// glfw function wrappers
static inline void swap_buffers() { glfwSwapBuffers(window); }
static inline void poll_events() { glfwPollEvents(); }
static inline void close_window() { glfwTerminate(); }
static inline bool window_should_close() { return glfwWindowShouldClose(window); }
static inline void clear_buffers()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif // XEN_H
