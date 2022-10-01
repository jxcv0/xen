#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "model.h"

#define MAX_CACHED_MESHES 64
#define MAX_CACHED_TEXTURES 128

/**
 * @brief Load a model from a file.
 *
 * @param filepath The filepath to the file.
 * @return A the new model containing mesh data.
 */
struct model *resources_load_model(const char *filepath);

#endif // RESOURCES_H_
