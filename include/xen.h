#ifndef XEN_H
#define XEN_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>

//window
static GLFWwindow* window;
static float window_w;
static float window_h;

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
        glGetProgramInfoLog(prgm_id, 1024, NULL, log)
        printf("Shader program linking error: %s\n", log);
    }
}

unsigned int load_shader(const char* vert_path, const char* frag_path)
{
    // TODO
}

#endif // XEN_H
