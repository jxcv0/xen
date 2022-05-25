#ifndef XEN_H
#define XEN_H

#include "glad.h"
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdio.h>
GLFWwindow* window;
float window_w;
float window_h;

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

    if(!window)
    {
        printf("Unable to create GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_resize);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
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

// TODO delete
void temp_esc()
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

#endif // XEN_H
