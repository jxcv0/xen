#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "model.h"

#define MAX_CACHED_MESHES 64
#define MAX_CACHED_TEXTURES 128

/**
 * @brief Import assets from a wavefront .obj file
 *
 * @param filepath The filepath to the file.
 * @return A model containing the asset data.
 */
struct model *resources_load_obj(const char *filepath);

#endif // RESOURCES_H_
