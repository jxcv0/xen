#ifndef XEN_H
#define XEN_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

//window
static GLFWwindow* window;
static float window_w;
static float window_h;

typedef struct vec2_t
{
    float values[2];
} vec2_t;

typedef struct vec3_t
{
    float values[3];
} vec3_t;

typedef struct vec4_t
{
    float values[4];
} vec4_t;

typedef struct mat4_t
{
    float values[4][4];
} mat4_t;

// TODO by value or ptr

vec4_t row(mat4_t m, int n)
{
    vec4_t result = {
        .values = {m.values[n][0], m.values[n][1], m.values[n][2], m.values[n][3]}
    };
    return result;
}

vec4_t col(mat4_t m, int n)
{
    vec4_t result = {
        .values = {m.values[0][n], m.values[1][n], m.values[2][n], m.values[3][n]}
    };
    return result;
}

float dot_v4(vec4_t v1, vec4_t v2)
{
    return (v1.values[0] * v2.values[0])
         + (v1.values[1] * v2.values[1])
         + (v1.values[2] * v2.values[2])
         + (v1.values[3] * v2.values[3]);
}

mat4_t cross_m4(mat4_t m1, mat4_t m2)
{
    mat4_t result;

    result.values[0][0] = dot_v4(row(m1, 0), col(m2, 0));
    result.values[0][1] = dot_v4(row(m1, 0), col(m2, 1));
    result.values[0][2] = dot_v4(row(m1, 0), col(m2, 2));
    result.values[0][3] = dot_v4(row(m1, 0), col(m2, 3));

    result.values[1][0] = dot_v4(row(m1, 1), col(m2, 0));
    result.values[1][1] = dot_v4(row(m1, 1), col(m2, 1));
    result.values[1][2] = dot_v4(row(m1, 1), col(m2, 2));
    result.values[1][3] = dot_v4(row(m1, 1), col(m2, 3));

    result.values[2][0] = dot_v4(row(m1, 2), col(m2, 0));
    result.values[2][1] = dot_v4(row(m1, 2), col(m2, 1));
    result.values[2][2] = dot_v4(row(m1, 2), col(m2, 2));
    result.values[2][3] = dot_v4(row(m1, 2), col(m2, 3));

    result.values[3][0] = dot_v4(row(m1, 3), col(m2, 0));
    result.values[3][1] = dot_v4(row(m1, 3), col(m2, 1));
    result.values[3][2] = dot_v4(row(m1, 3), col(m2, 2));
    result.values[3][3] = dot_v4(row(m1, 3), col(m2, 3));

    return result;
}

void on_resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init_window(float w, float h, const char* window_name)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window_w = w;
    window_h = h;
    window = glfwCreateWindow(window_w, window_h, window_name, NULL, NULL);

    if (!window)
    {
        printf("Unable to create GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_resize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Unable to initialize GLAD\n");
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void close_window()
{
    glfwTerminate();
}

void handle_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

bool should_close()
{
    return glfwWindowShouldClose(window);
}

void swap_buffers() { glfwSwapBuffers(window); }
void poll_events() { glfwPollEvents(); }

void shader_check_compile(GLuint shader_id)
{
    GLint success;
    GLchar log[1024];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader_id, 1024, NULL, log);
        printf("Shader program compilation error: %s\n", log);
    }
}

void shader_check_link(GLuint prgm_id)
{
    GLint success;
    GLchar log[1024];
    glGetProgramiv(prgm_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(prgm_id, 1024, NULL, log);
        printf("Shader program linking error: %s\n", log);
    }
}

unsigned int load_shader(const char* vert_path, const char* frag_path)
{
    // TODO
}

#endif // XEN_H
