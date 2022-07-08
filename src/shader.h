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

#ifndef SHADER_H
#define SHADER_H

#include "glad.h"

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

#endif // SHADER_H
