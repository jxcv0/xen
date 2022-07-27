#ifndef CAMERA_H
#define CAMERA_H

#include "maths.h"

#ifndef GL_INCLUDED
#define GL_INCLUDED
#include "glad.h"
#include <GLFW/glfw3.h>
#endif

// set the camera update function
void set_camera_update_callback(GLFWcursorposfun);

// create view matrix
mat4_t gen_view_matrix(void);

// update the camera direction based on a change in mouse position
// default version rotates about world origin
void camera_update_dir(GLFWwindow* window, double x, double y);

// update the camera direction based on a change in mouse position
// debug version rotates about camera_pos
void camera_update_dir_debug(GLFWwindow* window, double x, double y);

#endif // CAMERA_H
