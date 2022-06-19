/* 
 * Copyright (c) 2022 Jacob Micoud
 * Licensed under GPL-3.0-or-later
 */

#ifndef XEN_H
#define XEN_H

#define XEN_DEBUG

#include "lm.h"

#include "glad.h"
#include <GLFW/glfw3.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// #include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// global window - only need one of these
extern GLFWwindow* window;

// check for gl errs
GLenum checkerror_(const char *file, int line);

#define checkerr() checkerror_(__FILE__, __LINE__)

// opengl debug callback
void APIENTRY gl_debug_output(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char *message,
                              const void *userParam);

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
    mat4_t: shader_set_uniform_mat4 \
    )(x, y, z)

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

typedef struct light_t
{
    vec3_t color;
    vec3_t position;
    float constant;
    float linear;
    float quadratic;
} light_t;

light_t create_default_light()
{
    light_t result = {
        .color.values = {1.0f, 1.0f, 1.0f},
        .position.values = {1.0f, 3.0f, -1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032
    };
    return result;
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
unsigned int load_texture(const char* dir, const char* tex_name);

// for now we assume that each model has 3 texture maps in the same dir, the file is .obj and all 3
// textures are .png
int mesh_load(mesh_t* mesh, const char* dir, const char* name);

// load a mesh with no texture data
int mesh_load_simple(mesh_t* mesh, const char* filepath);

// free mesh memory
void free_mesh(mesh_t* mesh);

// draw a simple mesh with no textures
void draw_mesh_simple(mesh_t* mesh, unsigned int shader)

// draw a mesh
void draw_mesh(mesh_t* mesh, unsigned int shader);

// update the camera direction based on a change in mouse position
// default version rotates about world origin
void camera_update_dir(GLFWwindow* window, double x, double y);

// update the camera direction based on a change in mouse position
// debug version rotates about camera_pos
void camera_update_dir_debug(GLFWwindow* window, double x, double y);

// create a model matrix from a mesh position and rotation
mat4_t mesh_model_matrix(const mesh_t* mesh);

// generate a view matrix from the camera
mat4_t camera_view_matrix(void);

// window resize callback
void on_resize(GLFWwindow* window, int width, int height);

// initialize window and opengl
void xen_init();

// get input from glfw window
void handle_input(float delta_t);

// glfw function wrappers
void swap_buffers() { glfwSwapBuffers(window); }
void poll_events() { glfwPollEvents(); }
void close_window() { glfwTerminate(); }
bool window_should_close() { return glfwWindowShouldClose(window); }
void clear_buffers()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

#endif // XEN_H
