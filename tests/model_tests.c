#include "testing.h"
#include "xen.h"

mesh_t mesh;

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
    window_init(100, 100, "model_tests");
    load_mesh_obj(&mesh, "assets/models/ucube/", "ucube");

    vertices_test();
    return 0;
}
