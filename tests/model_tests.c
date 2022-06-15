#include "xen.h"
#include "testing.h"

mesh_t mesh;

void tex_coords_test()
{
    test_start();
    
    if (fabs(mesh.tex_coords->values[0] - 0.208008f) > FLT_EPSILON)
    {
        test_fail();
    }

    if (fabs(mesh.tex_coords->values[1] - 0.804321) > FLT_EPSILON)
    {
        test_fail();
    }

    test_pass();
}

int main(void)
{
    window_init(100.0f, 100.0f, "test");
    load_mesh(&mesh, "assets/models/cyborg/", "cyborg");

    tex_coords_test();
    return 0;
}
