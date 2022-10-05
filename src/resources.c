#include "resources.h"

#include <assimp/types.h>

#define STB_IMAGE_IMPLEMENTATION
#include <assimp/cimport.h>
#include <assimp/defs.h>
#include <assimp/material.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "3dmath.h"
#include "material.h"
#include "mesh.h"
#include "stb_image.h"
#include "texture.h"
#include "vertex.h"


