#ifndef MESH_H
#define MESH_H

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
    
    vec3_t world_position;
    float rot_b;
} mesh_t;

// load a texure from a file and add data to gl buffers
unsigned int load_texture(const char *dir, const char* tex_name);

// for now we assume that each model has 3 texture maps in the same dir, the file is .obj and all 3
// textures are .png
int mesh_load(mesh_t* mesh, const char *dir, const char* name);

// load a mesh with no texture data
int mesh_load_simple(mesh_t *mesh, const char* filepath);

// free mesh memory
void free_mesh(mesh_t *mesh);

// draw a simple mesh with no textures
void draw_mesh_simple(mesh_t *mesh, unsigned int shader);

// draw a mesh
void draw_mesh(mesh_t *mesh, unsigned int shader);


#endif //  MESH_H
