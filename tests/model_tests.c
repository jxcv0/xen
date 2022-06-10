#include "testing.h"
#include "xen.h"

mesh_t mesh;

// initialize window and opengl
void init_gl_only(void)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    glfwMakeContextCurrent(NULL);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        perror("Unable to initialize GLAD\n");
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    int flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl_debug_output, NULL);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    }
}
void vertices_test(void)
{
    test_start();
    float expected[] = {
	1.000000, 1.000000, -1.000000,
	1.000000, -1.000000, -1.000000,
	1.000000, 1.000000, 1.000000,
	1.000000, -1.000000, 1.000000,
	-1.000000, 1.000000, -1.000000,
	-1.000000, -1.000000, -1.000000,
	-1.000000, 1.000000, 1.000000,
	-1.000000, -1.000000, 1.000000,
    };

    for (int i = 0; i < mesh.n_vertices; i++)
    {
        print_vec3(mesh.positions[i]);
    }

    printf("\n");

    test_fail();
}

int main(void)
{
    init_gl_only();
    load_mesh_obj(&mesh, "assets/models/ucube/", "ucube");

    vertices_test();
    return 0;
}
