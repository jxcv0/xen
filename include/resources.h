#ifndef RESOURCES_H_
#define RESOURCES_H_

#include "mesh.h"
#include "texture.h"

/**
 * @brief Load a model and associated resources into the resource cache.
 *
 * @param filepath The filepath to the file.
 */
void re_load_model(const char *filepath);

struct model *re_fetch_model(const char *name);

#endif  // RESOURCES_H_
