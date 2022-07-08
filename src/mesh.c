#include "mesh.h"

#include "lm.h"
#include "shader.h"

#include <stdlib.h>

#include "glad.h"
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

// draw a mesh with textures
void draw_mesh(mesh_t* mesh, unsigned int shader)
{
    shader_use(shader);
    checkerr();

    if (mesh->tex_coords != NULL)
    {
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
        checkerr();
    }

    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);
    checkerr();

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    checkerr();
}

// draw a simple mesh with no textures
void draw_mesh_simple(mesh_t* mesh, unsigned int shader)
{
    glBindVertexArray(mesh->VAO);
    glDrawElements(GL_TRIANGLES, mesh->n_indices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

