#include "resources.h"

#include <stdint.h>

static uint32_t s_num_meshes;
static struct mesh *s_mesh_cache

static uint32_t s_num_textures;
static struct texture *s_texture_cache;

int load_mesh(const char *filepath) {
  if (num_meshes == MAX_MESHES) {
    return -1;
  }

  mesh_t *mesh = &meshes[num_meshes];

  // open file
  FILE *file = NULL;
  if ((file = fopen(filepath, "r")) == NULL) {
    perror(filepath);
    return -1;
  }

  int v_count = 0, vt_count = 0, vn_count = 0, f_count = 0;
  char *line = NULL;
  size_t len = 0;
  ssize_t nread = 0;
  while ((nread = getline(&line, &len, file)) !=
         -1)  // count the number of vertices
  {
    if (strstr(line, "v ") != NULL) {
      v_count++;
    } else if (strstr(line, "vt ") != NULL) {
      vt_count++;
    } else if (strstr(line, "vn ") != NULL) {
      vn_count++;
    } else if (strstr(line, "f ") != NULL) {
      f_count++;
    }
  }

  // temp storage
  vec3_t vertices[v_count];
  vec2_t texcoords[vt_count];
  vec3_t normals[vn_count];

  // allocate enough space for duplicating VTN sets
  mesh->num_vertices = f_count * 3;  // 3 vertices per face
  size_t vertices_size = sizeof(vec3_t) * mesh->num_vertices;
  size_t texcoords_size = sizeof(vec2_t) * mesh->num_vertices;
  size_t normals_size = sizeof(vec3_t) * mesh->num_vertices;
  size_t mem_block_size = vertices_size + texcoords_size + normals_size;
  mesh->mem_block = malloc(mem_block_size);
  if (mesh->mem_block == NULL) {
    mesh = NULL;
    return -1;
  }
  mesh->vertices = (vec3_t *)(mesh->mem_block);
  mesh->texcoords = (vec2_t *)(mesh->mem_block + vertices_size);
  mesh->normals = (vec3_t *)(mesh->mem_block + vertices_size + normals_size);
  v_count = 0;
  vt_count = 0;
  vn_count = 0;
  int it = 0;
  rewind(file);

  while ((nread = getline(&line, &len, file)) != -1) {
    char *linesave = NULL;
    char *token = strtok_r(line, " ", &linesave);
    if (strncmp(token, "#", 2) == 0) {  // comments
      continue;
    } else if (strncmp(token, "mtllib", 6) == 0) {  // material library
      // TODO handle materials
      continue;
    } else if (strncmp(token, "o", 2) == 0) {  // object name
      continue;
    } else if (strncmp(token, "v", 2) == 0) {  // vertices
      for (int i = 0;; i++) {
        token = strtok_r(NULL, " ", &linesave);
        if (token == NULL) {
          break;
        }
        vertices[v_count].values[i] = strtof(token, NULL);
      }
      v_count++;
    } else if (strncmp(token, "vt", 2) == 0) {  // texcoords
      for (int i = 0;; i++) {
        token = strtok_r(NULL, " ", &linesave);
        if (token == NULL) {
          break;
        }
        texcoords[vt_count].values[i] = strtof(token, NULL);
      }
      vt_count++;
    } else if (strncmp(token, "vn", 2) == 0) {  // normals
      for (int i = 0;; i++) {
        token = strtok_r(NULL, " ", &linesave);
        if (token == NULL) {
          break;
        }
        normals[vn_count].values[i] = strtof(token, NULL);
      }
      vn_count++;
    } else if (strncmp(token, "s", 2) == 0) {  // smooth shading always off
      continue;
    } else if (strncmp(token, "f", 2) == 0) {  // faces
      char *toksave = NULL;
      for (; it < f_count; it++) {
        token = strtok_r(NULL, " ", &linesave);
        if (token == NULL) {
          break;
        }
        int i = 0;
        int index[3];
        for (char *tok = token;; i++, tok = NULL) {
          char *subtok = strtok_r(tok, "/", &toksave);
          if (subtok == NULL) {
            break;
          }
          index[i] = atoi(subtok) - 1;
        }
        mesh->vertices[it] = vertices[index[0]];
        mesh->texcoords[it] = texcoords[index[1]];
        mesh->normals[it] = normals[index[2]];
      }
    } else if (strncmp(token, "mtllib", 6) == 0) {
      // TODO handle materials
      continue;
    }
  }
  free(line);
  fclose(file);
  return num_meshes++;
}
