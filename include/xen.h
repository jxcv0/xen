#ifndef XEN_H
#define XEN_H

#include "glad.h"
#include <GLFW/glfw3.h>

// #include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

//window
static GLFWwindow* window;
static float window_w;
static float window_h;

typedef struct vec2_t
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float values[2];
    };
} vec2_t;

typedef struct vec3_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
        };
        float values[3];
    };
} vec3_t;

typedef struct vec4_t
{
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
    float values[4];
} vec4_t;

typedef struct mat4_t
{
    float values[4][4];
} mat4_t;

mat4_t identity_mat4()
{
    mat4_t result;
    result.values[0][0] = 1.0f;
    result.values[1][1] = 1.0f;
    result.values[2][2] = 1.0f;
    result.values[3][3] = 1.0f;
    return result;
}

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
    return (v1.x * v2.x)
         + (v1.y * v2.y)
         + (v1.z * v2.z)
         + (v1.w * v2.w);
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

typedef struct mesh_t
{
    // TODO allocations
    unsigned int VAO, VBO, EBO;
    vec3_t* positions;
    vec3_t* normals;
    vec2_t* tex_coords;
    vec3_t* tangents;
    vec3_t* bitangents;
    int* tex_ids;
    char* uniform_names;
} mesh_t;

int load_model(const char* model_path)
{
    // TODO do in dedicated asset streaming thread
    // TODO return handle to array or return model_t?

    FILE* fstream;
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;

    int n_vertices = 0;
    int n_texcoords = 0;
    int n_normals = 0;
    int n_indices = 0;

    fstream = fopen(model_path, "r");
    if (fstream == NULL)
    {
        fprintf(stderr, "Unable to open .obj file | %s\n", model_path);
    }

    // count
    while ((nread = getline(&line, &len, fstream)) != -1)
    {
        // ignore comments
        if (line[0] == '#') { continue; }

        // mtlib
        // TODO

        // o
        // TODO

        // vertices
        if (line[0] == 'v' && line[1] == ' ')
        {
            // TODO
            continue;
        }

        // textures
        // normals
        // indices

        // usemtl
        // TODO
        
        // s
        // TODO

        printf("%s", line);
    }

    free(line);
    fclose(fstream);

    return 0;
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
        perror("Unable to create GLFW window\n");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, on_resize);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        perror("Unable to initialize GLAD\n");
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

unsigned int load_shader(const char* vert_path, const char* frag_path)
{
    FILE* file;
    char* vert_code;
    char* frag_code;

    // vert
    if ((file = fopen(vert_path, "r")) != NULL)
    {
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
    }
    else
    {
        fprintf(stderr, "unable to open vertex shader file | %s", vert_path);
        return -1;
    }

    // frag
    if ((file = fopen(frag_path, "r")) != NULL)
    {
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
    }
    else
    {
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

void use_shader(unsigned int shader)
{
    glUseProgram(shader);
}

#endif // XEN_H
