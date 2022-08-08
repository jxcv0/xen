#include <stdio.h>


static void window_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// init glfw, opengl and open game window
void window_init(void)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	window = glfwCreateWindow(scr_width, scr_height, "XEN", NULL, NULL);

	if (!window) {
		perror("Unable to create GLFW window\n");
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, window_resize_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		perror("Unable to initialize GLAD\n");
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwWindowHint(GLFW_SAMPLES, 4);

}

// close game window
void window_close(void)
{
	glfwTerminate();
}

// set glfw flag
void set_window_should_close(bool flag)
{
	glfwSetWindowShouldClose(window_ptr(), flag);
}

// get glfw flag
bool window_should_close(void)
{
	return glfwWindowShouldClose(window);
}

// swap buffers
void window_swap_buffers(void)
{
	glfwSwapBuffers(window);
}

// get ptr to the game window
GLFWwindow* window_ptr(void)
{
	// syncronization?
	return window;
}


// calculate perspective matrix
mat4_t window_perspective_matrix(float fov)
{
	return perspective(radians(fov), 0.1f, 100.0f, (scr_width/scr_height));
}
