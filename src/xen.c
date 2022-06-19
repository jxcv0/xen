/* 
 * Copyright (c) 2022 Jacob Micoud
 * Licensed under GPL-3.0-or-later
 */

#define XEN_DEBUG
#include "xen.h"

#include <stdio.h>

// window
GLFWwindow* window;
static float screen_w = 800.0f;
static float screen_h = 600.0f;

// camera
static vec3_t camera_pos = { .values = {0.0f, 0.0f, 0.0f} };
static vec3_t camera_dir = { .values = {0.0f, 0.0f, -1.0f} };
static vec3_t camera_up = { .values = {0.0f, 1.0f, 0.0f} };
static vec3_t camera_right = { .values = {1.0f, 0.0f, 0.0f} };
static vec3_t world_up = { .values = {0.0f, 1.0f, 0.0f} };
static bool camera_movement_debug = false;
static bool first_mouse = true;
static float rot_a = 0.0f;  // rotation about x axis
static float rot_b = -90.0f; // rotation about y axis
static float prev_x = 0;
static float prev_y = 0;
static float offset_rad = 3.0f;

// check for gl errs
GLenum checkerror_(const char *file, int line)
{
    GLenum errorCode = GL_NO_ERROR;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error = NULL;
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

// opengl debug callback
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
// check compile status
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

// check link status
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

// load and compile a shader from a text file
unsigned int shader_load(const char* vert_path, const char* frag_path)
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

// for now we assume that each model has 3 texture maps in the same dir, the file is .obj and all 3
// textures are .png
// TODO lots of strange string things going on here
int mesh_load(mesh_t* mesh, const char* dir, const char* name)
{
    char filepath[strlen(dir) + strlen(name)];
    strcpy(filepath, dir);
    strcat(filepath, name);
    strcat(filepath, ".obj");

    const struct aiScene* scene = aiImportFile(filepath,
        aiProcess_Triangulate      |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs          |
        aiProcess_CalcTangentSpace );

    if (!scene)
    {
        fprintf(stderr, "Unable to open model file | %s\n", filepath);
        return 1;
    }

    mesh->world_position = construct_vec3(0.0f, 0.0f, 0.0f);
    mesh->rot_b = 180.0f;   // facing -z

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

    size_t mem_size = positions_size
                    + normals_size
                    + tex_coords_size
                    + tangents_size
                    + bitangents_size
                    + indices_size;

    mesh->mem_block = malloc(mem_size);

    // memory offsets into mesh memory
    mesh->positions = (vec3_t*)(mesh->mem_block);

    mesh->normals = (vec3_t*)(mesh->mem_block
                            + positions_size);

    mesh->tex_coords = (vec2_t*)(mesh->mem_block
                               + positions_size
                               + normals_size);

    mesh->tangents = (vec3_t*)(mesh->mem_block
                             + positions_size
                             + normals_size
                             + tex_coords_size);

    mesh->bitangents = (vec3_t*)(mesh->mem_block
                               + positions_size
                               + normals_size
                               + tex_coords_size
                               + tangents_size);

    mesh->indices = (int*)(mesh->mem_block
                         + positions_size
                         + normals_size
                         + tex_coords_size
                         + tangents_size
                         + bitangents_size);

    for(int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        mesh->positions[i].values[0] = ai_mesh->mVertices[i].x;
        mesh->positions[i].values[1] = ai_mesh->mVertices[i].y;
        mesh->positions[i].values[2] = ai_mesh->mVertices[i].z;

        mesh->normals[i].values[0] = ai_mesh->mNormals[i].x;
        mesh->normals[i].values[1] = ai_mesh->mNormals[i].y;
        mesh->normals[i].values[2] = ai_mesh->mNormals[i].z;
    
        if (ai_mesh->mTextureCoords[0])
        {
            mesh->tex_coords[i].values[0] = ai_mesh->mTextureCoords[0][i].x; 
            mesh->tex_coords[i].values[1] = ai_mesh->mTextureCoords[0][i].y;

            mesh->tangents[i].values[0] = ai_mesh->mTangents[i].x;
            mesh->tangents[i].values[1] = ai_mesh->mTangents[i].y;
            mesh->tangents[i].values[2] = ai_mesh->mTangents[i].z;

            mesh->bitangents[i].values[0] = ai_mesh->mBitangents[i].x;
            mesh->bitangents[i].values[1] = ai_mesh->mBitangents[i].y;
            mesh->bitangents[i].values[2] = ai_mesh->mBitangents[i].z;
        }
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
    if (ai_mesh->mTextureCoords[0])
    {
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
    }

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
    ptrdiff_t offset = (void*)mesh->positions - mesh->mem_block;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
    
    // normals
    offset = (void*)mesh->tex_coords - mesh->mem_block;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // tex_coords
    offset = (void*)mesh->normals - mesh->mem_block;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // tangent
    offset = (void*)mesh->tangents - mesh->mem_block;
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // bitangent
    offset = (void*)mesh->bitangents - mesh->mem_block;
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // unbind
    glBindVertexArray(0);
    return 0;
}

// load a mesh with no texture data
int mesh_load_simple(mesh_t* mesh, const char* filepath)
{
    mesh->tex_coords = NULL;
    mesh->tangents = NULL;
    mesh->bitangents = NULL;

    const struct aiScene* scene = aiImportFile(filepath,
        aiProcess_Triangulate      |
        aiProcess_GenSmoothNormals |
        aiProcess_FlipUVs          |
        aiProcess_CalcTangentSpace );

    if (!scene)
    {
        fprintf(stderr, "Unable to open model file | %s\n", filepath);
        return 1;
    }

    mesh->world_position = construct_vec3(0.0f, 0.0f, 0.0f);
    mesh->rot_b = 180.0f;   // facing -z

    struct aiMesh* ai_mesh = scene->mMeshes[0];
    mesh->n_vertices = ai_mesh->mNumVertices;
    mesh->n_indices = ai_mesh->mNumFaces * 3;    // all faces are triangulated

    // allocate block of memory for mesh
    size_t positions_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t normals_size = sizeof(vec3_t) * mesh->n_vertices;
    size_t indices_size = sizeof(int) * mesh->n_indices;

    size_t mem_size = positions_size
                    + normals_size
                    + indices_size;

    mesh->mem_block = malloc(mem_size);

    // memory offsets into mesh memory
    mesh->positions = (vec3_t*)(mesh->mem_block);

    mesh->normals = (vec3_t*)(mesh->mem_block
                            + positions_size);

    mesh->tex_coords = (vec2_t*)(mesh->mem_block
                               + positions_size
                               + normals_size);

    mesh->indices = (int*)(mesh->mem_block
                         + positions_size
                         + normals_size);

    for(int i = 0; i < ai_mesh->mNumVertices; i++)
    {
        mesh->positions[i].values[0] = ai_mesh->mVertices[i].x;
        mesh->positions[i].values[1] = ai_mesh->mVertices[i].y;
        mesh->positions[i].values[2] = ai_mesh->mVertices[i].z;

        mesh->normals[i].values[0] = ai_mesh->mNormals[i].x;
        mesh->normals[i].values[1] = ai_mesh->mNormals[i].y;
        mesh->normals[i].values[2] = ai_mesh->mNormals[i].z;
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
    ptrdiff_t offset = (void*)mesh->positions - mesh->mem_block;
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);
    
    // normals
    offset = (void*)mesh->tex_coords - mesh->mem_block;
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // tex_coords
    offset = (void*)mesh->normals - mesh->mem_block;
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)offset);

    // unbind
    glBindVertexArray(0);
    return 0;
}

// free mesh memory
void free_mesh(mesh_t* mesh)
{
    glDeleteVertexArrays(1, &mesh->VAO);
    glDeleteBuffers(1, &mesh->VBO);
    free(mesh->mem_block);
}

// draw a mesh
void draw_mesh(mesh_t* mesh, unsigned int shader)
{
    shader_use(shader);

    // diff
    glActiveTexture(GL_TEXTURE0);
    shader_set_uniform(shader, "tex_diff", 0);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[0]);

    // spec
    glActiveTexture(GL_TEXTURE1);
    shader_set_uniform(shader, "tex_spec", 1);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[1]);

    // norm
    glActiveTexture(GL_TEXTURE2);
    shader_set_uniform(shader, "tex_norm", 2);
    glBindTexture(GL_TEXTURE_2D, mesh->tex_ids[2]);

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

// update the camera direction based on a change in mouse position
// default version rotates about world origin
void camera_update_dir(GLFWwindow* window, double x, double y)
{
    float mouse_x = (float)x;
    float mouse_y = (float)y;

    if (first_mouse)
    {
        prev_x = mouse_x;
        prev_y = mouse_y;
        first_mouse = false;
    }

    float delta_x = mouse_x - prev_x;
    float delta_y = prev_y - mouse_y;

    prev_x = mouse_x;
    prev_y = mouse_y;

    // TODO make sensitivity adjustable
    rot_b += delta_x * 0.1f;
    rot_a += delta_y * 0.1f;

    if (rot_a > 89.0f) { rot_a = 89.0f; }
    if (rot_a < -89.0f) { rot_a = -89.0f; }

    float rads_a = radians(rot_a);
    float rads_b = radians(rot_b);

    camera_pos = construct_vec3(-offset_rad * (cos(rads_b) * cos(rads_a)),
                                -offset_rad * (sin(rads_a)),
                                -offset_rad * (sin(rads_b) * cos(rads_a)));

    camera_pos.values[1] += 3.5f;

    camera_dir = construct_vec3(cos(rads_b) * cos(rads_a),
                                sin(rads_a),
                                sin(rads_b) * cos(rads_a));
    
    camera_dir = normalize_vec3(camera_dir);
    camera_right = normalize_vec3(cross_vec3(world_up, camera_dir));
    camera_up = normalize_vec3(cross_vec3(camera_dir, camera_right));
}

// update the camera direction based on a change in mouse position
// debug version rotates about camera_pos
void camera_update_dir_debug(GLFWwindow* window, double x, double y)
{
    float mouse_x = (float)x;
    float mouse_y = (float)y;

    if (first_mouse)
    {
        prev_x = mouse_x;
        prev_y = mouse_y;
        first_mouse = false;
    }

    float delta_x = mouse_x - prev_x;
    float delta_y = prev_y - mouse_y;

    prev_x = mouse_x;
    prev_y = mouse_y;

    // TODO make sensitivity adjustable
    rot_b += delta_x * 0.1f;
    rot_a += delta_y * 0.1f;

    if (rot_a > 89.0f) { rot_a = 89.0f; }
    if (rot_a < -89.0f) { rot_a = -89.0f; }

    float rads_a = radians(rot_a);
    float rads_b = radians(rot_b);

    camera_dir = construct_vec3(cos(rads_b) * cos(rads_a),
                                sin(rads_a),
                                sin(rads_b) * cos(rads_a));

    camera_dir = normalize_vec3(camera_dir);
    camera_right = normalize_vec3(cross_vec3(world_up, camera_dir));
    camera_up = normalize_vec3(cross_vec3(camera_dir, camera_right));
}

// create a model matrix from a mesh position and rotation
mat4_t mesh_model_matrix(const mesh_t* mesh)
{
        mat4_t m = construct_mat4(1.0f);
        m = translate(m, mesh->world_position); return rotate(m, world_up, mesh->rot_b);
}

// generate a view matrix from the camera
mat4_t camera_view_matrix(void)
{
    vec3_t dir = add_vec3(camera_pos, camera_dir);
    return look_at(camera_pos, dir, camera_up);
}

// window resize callback
void on_resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void xen_init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    window = glfwCreateWindow(800.0f, 600.0f, "KV", NULL, NULL);

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
    glfwSetCursorPosCallback(window, camera_update_dir);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);

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

// get input from glfw window
void handle_input(float delta_t)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return;
    }

    // debug camera
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        camera_movement_debug = !camera_movement_debug;
        if (camera_movement_debug)
        {
            glfwSetCursorPosCallback(window, camera_update_dir_debug);
        }
        else
        {
            glfwSetCursorPosCallback(window, camera_update_dir);
        }
    }

    if (camera_movement_debug)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(camera_dir, delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(camera_dir, -delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(camera_right, -delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(camera_right, delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(world_up, delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            vec3_t dist = scale_vec3(world_up, -delta_t * 2.0f);
            camera_pos = add_vec3(camera_pos, dist);
        }

        return;
    }
    
    // TODO player controls
}

// main setup and game loop
int main()
{
    // window
    xen_init();

    // shader
    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);
    
    // mesh
    mesh_t mesh;
    mesh_load_simple(&mesh, "assets/models/ucube/ucube.obj");
    checkerr();

    // projection matrix
    mat4_t p_mat = perspective(55.0f, 0.1f, 100.0f, (screen_w/screen_h));
    shader_set_uniform(shader, "projection", p_mat);
    checkerr();

    light_t light = create_default_light();

    float last_frame = 0.0f;
    
    camera_update_dir(window, (double)screen_w/2.0, (double)screen_h/2.0);

    // TODO will be producer loop
    while(!window_should_close())
    {
        float current_frame = (float)glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        handle_input(delta_time);

        // view matrix
        shader_set_uniform(shader, "view", camera_view_matrix());
        shader_set_uniform(shader, "view_pos", camera_pos);

        // model matrix
        shader_set_uniform(shader, "model", mesh_model_matrix(&mesh));

        // light
        shader_set_uniform(shader, "light.color", light.color);
        shader_set_uniform(shader, "light.position", light.position);
        shader_set_uniform(shader, "light.constant", light.constant);
        shader_set_uniform(shader, "light.linear", light.linear);
        shader_set_uniform(shader, "light.quadratic", light.quadratic);
        shader_set_uniform(shader, "shininess", 8.0f);

        // draw
        clear_buffers();
        draw_mesh(&mesh, shader);

        swap_buffers();
        poll_events();
        checkerr();
    }

    free_mesh(&mesh);
    close_window();

    return 0;
}
