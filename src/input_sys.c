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
		XEN_INPUT_BUFFER |= (uint8_t)KEY_F1;
	}

	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		XEN_INPUT_BUFFER |= (uint8_t)KEY_ESC;
	}
}

// clear the input buffer
void input_clear_buffer()
{
	XEN_INPUT_BUFFER = KEY_NONE;
}

// check if a key is pressed
bool input_key_pressed(enum KEY k)
{
	return (XEN_INPUT_BUFFER & (uint8_t)k);
}

#ifdef XEN_DEBUG
// debug/testing utility
void input_set_buffer(uint8_t i)
{
	XEN_INPUT_BUFFER |= i;
}
#endif // XEN_DEBUG
