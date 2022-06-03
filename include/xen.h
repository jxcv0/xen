#ifndef XEN_H
#define XEN_H

#include "vec.h"
#include "shader.h"

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

//window
static GLFWwindow* window;
static float window_w;
static float window_h;

GLenum checkerror_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
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

void APIENTRY gl_debug_output(GLenum source,
                              GLenum type,
                              unsigned int id,
                              GLenum severity,
                              GLsizei length,
                              const char *message,
                              const void *userParam)
{
    // non-significant error codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    printf("---------------\n");
    printf("Debug message (%d): %s\n", id, message);
    switch (source)
    {
        case GL_DEBUG_SOURCE_API: printf("Source: API"); break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM: printf("Source: Window System"); break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: printf("Source: Shader Compiler"); break;
        case GL_DEBUG_SOURCE_THIRD_PARTY: printf("Source: Third Party"); break;
        case GL_DEBUG_SOURCE_APPLICATION: printf("Source: Application"); break;
        case GL_DEBUG_SOURCE_OTHER: printf("Source: Other"); break;
    }
    printf("\n");

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR: printf("Type: Error"); break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: printf("Type: Deprecated Behaviour"); break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: printf("Type: Undefined Behaviour"); break;
        case GL_DEBUG_TYPE_PORTABILITY: printf("Type: Portability"); break;
        case GL_DEBUG_TYPE_PERFORMANCE: printf("Type: Performance"); break;
        case GL_DEBUG_TYPE_MARKER: printf("Type: Marker"); break;
        case GL_DEBUG_TYPE_PUSH_GROUP: printf("Type: Push Group"); break;
        case GL_DEBUG_TYPE_POP_GROUP: printf("Type: Pop Group"); break;
        case GL_DEBUG_TYPE_OTHER: printf("Type: Other"); break;
    }
    printf("\n");

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH: printf("Severity: high"); break;
        case GL_DEBUG_SEVERITY_MEDIUM: printf("Severity: medium"); break;
        case GL_DEBUG_SEVERITY_LOW: printf("Severity: low"); break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: printf("Severity: notification"); break;
    }
    printf("\n\n");
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
} mesh_t;

unsigned int load_texture(const char* dir, const char* tex_name)
{
    char filepath[strlen(dir) + strlen(tex_name)];
    strcpy(filepath, dir);
    strcat(filepath, tex_name);

    unsigned int tex_id = 0;
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
        // glGenerateMipMap(GL_TEXTURE_2D);

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

// for now we assume that each model has 3 texture maps in the same dir, the file is .obj and all 3 textures are .png
// TODO lots of strange string things going on here
int load_mesh_obj(mesh_t* mesh, const char* dir, const char* name)
{
    char filepath[strlen(dir) + strlen(name)];
    strcpy(filepath, dir);
    strcat(filepath, name);
    strcat(filepath, ".obj");

    const struct aiScene* scene = aiImportFile(filepath,
        aiProcess_CalcTangentSpace      | 
        aiProcess_Triangulate           |
        aiProcess_JoinIdenticalVertices );

    if (!scene)
    {
        fprintf(stderr, "Unable to open model file | %s\n", filepath);
        return 1;
    }

    struct aiMesh* ai_mesh = scene->mMeshes[0];
    mesh->n_vertices = ai_mesh->mNumVertices;
    mesh->n_indices = ai_mesh->mNumFaces * 3;    // all faces are triangulated

    // allocate block of memory for mesh
    size_t positions_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t normals_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t tex_coords_size = sizeof(vec2_t) * mesh->n_vertices;
    size_t tangents_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t bitangents_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t indices_size = sizeof(int) * mesh->n_indices;

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

    int n = 0;
    for(int i = 0; i < ai_mesh->mNumFaces; i++)
    {
        struct aiFace face = ai_mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++)
        {
            mesh->indices[n++] = face.mIndices[j];
        }
    }

    aiReleaseImport(scene);

    // textures
    size_t namelen = strlen(name);

    char diff[namelen + 9]; // name + "_type.png"
    strcpy(diff, name);
    strcat(diff, "_diff.png");

    char spec[namelen + 9];
    strcpy(spec, name);
    strcat(spec, "_spec.png");

    char norm[namelen + 9];
    strcpy(norm, name);
    strcat(norm, "_norm.png");

    mesh->tex_ids[0] = load_texture(dir, diff);
    mesh->tex_ids[1] = load_texture(dir, spec);
    mesh->tex_ids[2] = load_texture(dir, norm);

    // gl buffers
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, mem_size, mesh->mem_block, GL_STATIC_DRAW);

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->n_indices * sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

    // positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->positions);
    
    // normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)mesh->normals);

    // tex_coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->tex_coords);

    // tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->tangents);

    // bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)mesh->bitangents);

    // unbind
    glBindVertexArray(0);

    return 0;
}

void free_mesh(mesh_t* mesh)
{
    free(mesh->mem_block);
}

void draw_mesh(mesh_t* mesh, unsigned int shader)
{
    // diff
    glActiveTexture(GL_TEXTURE1);
    shader_set_uniformi(shader, "tex_diff", 0);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[0]);

    // spec
    glActiveTexture(GL_TEXTURE2);
    shader_set_uniformi(shader, "tex_spec", 1);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[1]);

    // norm
    glActiveTexture(GL_TEXTURE3);
    shader_set_uniformi(shader, "tex_norm", 2);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[2]);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
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


#endif // XEN_H
