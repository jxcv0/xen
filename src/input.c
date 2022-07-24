#include "input.h"

#include "window.h"

#ifdef XEN_DEBUG
#include <stdio.h>
#include <assert.h>
#endif // XEN_DEBUG

static int XEN_INPUT_BUFFER;

// get input from glfw window
void input_update_buffer()
{
	GLFWwindow* window = window_ptr();
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
