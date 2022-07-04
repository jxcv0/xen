#include "input_sys.h"

#include <GLFW/glfw3.h>

#ifdef XEN_DEBUG
#include <stdio.h>
#include <assert.h>
#endif // XEN_DEBUG

extern GLFWwindow *window; // defined in xen.c

static int XEN_INPUT_BUFFER;

// get input from glfw window
void input_update_buffer()
{
	if(glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		XEN_INPUT_BUFFER |= (int)F1;
#ifdef XEN_DEBUG
		assert(XEN_INPUT_BUFFER | 0x0100);
#endif // XEN_DEBUG
	}

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		XEN_INPUT_BUFFER |= (int)ESC;
#ifdef XEN_DEBUG
		assert(XEN_INPUT_BUFFER | 0x0080);
#endif // XEN_DEBUG
	}
}

// clear the input buffer
void input_clear_buffer()
{
	XEN_INPUT_BUFFER = 0;
}

// check if a key is pressed
bool input_key_pressed(enum KEY k)
{
	return (XEN_INPUT_BUFFER & (int)k) == 0;
}
