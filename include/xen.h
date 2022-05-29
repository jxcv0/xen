#ifndef XEN_H
#define XEN_H

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

#include "stb_image.h"

//window
static GLFWwindow* window;
static float window_w;
static float window_h;

GLenum checkerror_(const char *file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR){
        const char* error;
        switch (errorCode) {
            case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW: error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW: error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY: error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
            fprintf(stderr, "%s | %s (%d)\n", error, file, line);
        }
        return errorCode;
    }
#define checkerr() checkerror_(__FILE__, __LINE__)

/**
 * @brief Debug callback for opengl
 * 
 * @param source 
 * @param type 
 * @param id 
 * @param severity 
 * @param length 
 * @param message 
 * @param userParam 
 */
void APIENTRY gl_debug_output(
    GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char *message,
    const void *userParam
) {
    // ignore non-significant error codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
        printf("---------------\n");
        printf("Debug message (%d): %s\n", id, message);
        switch (source) {
            case GL_DEBUG_SOURCE_API: printf("Source: API"); break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: printf("Source: Window System"); break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: printf("Source: Third Party"); break;
            case GL_DEBUG_SOURCE_APPLICATION: printf("Source: Application"); break;
            case GL_DEBUG_SOURCE_OTHER: printf("Source: Other"); break;
        } printf("\n");
    
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: printf("Type: Error"); break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: printf("Type: Undefined Behaviour"); break;
            case GL_DEBUG_TYPE_PORTABILITY: printf("Type: Portability"); break;
            case GL_DEBUG_TYPE_PERFORMANCE: printf("Type: Performance"); break;
            case GL_DEBUG_TYPE_MARKER: printf("Type: Marker"); break;
            case GL_DEBUG_TYPE_PUSH_GROUP: printf("Type: Push Group"); break;
            case GL_DEBUG_TYPE_POP_GROUP: printf("Type: Pop Group"); break;
            case GL_DEBUG_TYPE_OTHER: printf("Type: Other"); break;
        } printf("\n");
    
        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH: printf("Severity: high"); break;
            case GL_DEBUG_SEVERITY_MEDIUM: printf("Severity: medium"); break;
            case GL_DEBUG_SEVERITY_LOW: printf("Severity: low"); break;
            case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
        } printf("\n");
    printf("\n");
}

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
        float values[4];
    };
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
    char uniform_names[3][4];
} mesh_t;

int load_mesh(mesh_t* mesh, const char* filepath)
{
    // TODO use own loading procedure 

    const struct aiScene* scene = aiImportFile(filepath,
        aiProcess_CalcTangentSpace       | 
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  );

    if(!scene)
    {
        fprintf(stderr, "Unable to open model file | %s\n", filepath);
        return 1;
    }

    struct aiMesh* ai_mesh = scene->mMeshes[0];
    mesh->n_vertices = ai_mesh->mNumVertices;
    mesh->n_indices = ai_mesh->mNumFaces * 3;    // all faces are triangulated

    // allocate block of memory for mesh
    int positions_size = sizeof(vec3_t) * mesh->n_vertices;
    int normals_size = sizeof(vec3_t) * mesh->n_vertices;
    int tex_coords_size = sizeof(vec2_t) * mesh->n_vertices;
    int tangents_size = sizeof(vec3_t) * mesh->n_vertices;
    int bitangents_size = sizeof(vec3_t) * mesh->n_vertices;
    int indices_size = sizeof(int) * mesh->n_indices;

    size_t mem_size = positions_size + normals_size + tex_coords_size + tangents_size + bitangents_size + indices_size;
    mesh->mem_block = malloc(mem_size);

    // memory offsets into mesh memory
    mesh->positions = (vec3_t*)mesh->mem_block;
    mesh->normals = (vec3_t*)(mesh->mem_block + positions_size);
    mesh->tex_coords = (vec2_t*)(mesh->mem_block + positions_size + normals_size);
    mesh->tangents = (vec3_t*)(mesh->mem_block + positions_size + normals_size + tex_coords_size);
    mesh->bitangents = (vec3_t*)(mesh->mem_block + positions_size + normals_size + tex_coords_size + tangents_size);
    mesh->indices = (int*)(mesh->mem_block + positions_size + normals_size + tex_coords_size + tangents_size + bitangents_size);

    for(int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        mesh->positions[i].x = ai_mesh->mVertices[i].x;
        mesh->positions[i].y = ai_mesh->mVertices[i].y;
        mesh->positions[i].z = ai_mesh->mVertices[i].z;

        mesh->normals[i].x = ai_mesh->mNormals[i].x;
        mesh->normals[i].y = ai_mesh->mNormals[i].y;
        mesh->normals[i].z = ai_mesh->mNormals[i].z;

        mesh->tex_coords[i].x = ai_mesh->mTextureCoords[0][i].x; 
        mesh->tex_coords[i].y = ai_mesh->mTextureCoords[0][i].y;

        mesh->tangents[i].x = ai_mesh->mTangents[i].x;
        mesh->tangents[i].y = ai_mesh->mTangents[i].y;
        mesh->tangents[i].z = ai_mesh->mTangents[i].z;

        mesh->bitangents[i].x = ai_mesh->mBitangents[i].x;
        mesh->bitangents[i].y = ai_mesh->mBitangents[i].y;
        mesh->bitangents[i].z = ai_mesh->mBitangents[i].z;
    }

    int face_counter = 0;
    for(int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        struct aiFace face = ai_mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
        {
            mesh->indices[face_counter++] = face.mIndices[j];
        }
    }

    aiReleaseImport(scene);

    // TODO materials and textures

    // gl buffers
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mem_size, mesh->mem_block, GL_STATIC_DRAW);

    checkerr();
    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_indices * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

    checkerr();
    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->positions);
    
    checkerr();
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)mesh->normals);

    checkerr();
    // tex_coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->tex_coords);

    checkerr();
    // tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->tangents);

    checkerr();
    // tangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->bitangents);

    glBindVertexArray(0);

    return 0;
}

void free_mesh(mesh_t* mesh)
{
    free(mesh->mem_block);
}

// TODO shader as arg?
void draw_mesh(mesh_t* mesh)
{
    // TODO
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

#ifdef XEN_DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }
#endif
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
