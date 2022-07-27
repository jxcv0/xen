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

#include "xen.h"

#include "input.h"
#include "window.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// create a light
light_t create_default_light()
{
    light_t result = {
        .color.values = {1.0f, 1.0f, 1.0f},
        .position.values = {1.0f, 1.0f, -1.0f},
        .constant = 1.0f,
        .linear = 0.09f,
        .quadratic = 0.032
    };
    return result;
}

// set shader uniform utility function
void shader_set_uniform_light(unsigned int shader,
                                unsigned int light_index,
                                const light_t* light)
{
    char color[16];
    sprintf(color, "lights[%d].color", light_index);

    char position[19];
    sprintf(position, "lights[%d].position", light_index);

    char constant[19];
    sprintf(constant, "lights[%d].constant", light_index);

    char linear[17];
    sprintf(linear, "lights[%d].linear", light_index);

    char quadratic[20];
    sprintf(quadratic, "lights[%d].quadratic", light_index);

    // shader_set_uniform(shader, color, light->color);
    // shader_set_uniform(shader, position, light->position);
    // shader_set_uniform(shader, constant, light->constant);
    // shader_set_uniform(shader, linear, light->linear);
    // shader_set_uniform(shader, quadratic, light->quadratic);
}

// load a texure from a file and add data to gl buffers
unsigned int load_texture(const char* dir, const char* tex_name)
{
    char filepath[strlen(dir) + strlen(tex_name)];
    strcpy(filepath, dir);
    strcat(filepath, tex_name);

    unsigned int tex_id;
    glGenTextures(1, &tex_id);

    int w, h, n;
    unsigned char* data = stbi_load(filepath, &w, &h, &n, 0);

    if (data)
    {
        GLenum format = GL_RGBA;

        if (n == 1)
        {
            format = GL_RED;
        }
        else if (n == 3)
        {
            format = GL_RGB;
        }

        glBindTexture(GL_TEXTURE_2D, tex_id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        // glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
	}
    else
    {
        fprintf(stderr, "Unable to load texture | %s\n", filepath);
        stbi_image_free(data);
    }
    return tex_id;
}

void frame_begin()
{
	input_update_buffer();
}

void frame_end()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// graphics_draw_objects();
	// TODO jobsys_wait();
	window_swap_buffers();
	glfwPollEvents();
	input_clear_buffer();
}
