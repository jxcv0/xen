#include "xen.h"
#include "testing.h"

mesh_t mesh;

void tex_coords_test()
{
    test_start();
    
    if (fabs(mesh.tex_coords[0].values[0] - 0.208008f) > FLT_EPSILON)
    {
        test_fail();
        return;
    }

    if (fabs(mesh.tex_coords[0].values[1] - 0.804321) > FLT_EPSILON)
    {
        test_fail();
        return;
    }
    
    // assimp changes order?

    /*
    if (fabs(mesh.tex_coords[mesh.n_vertices].values[0] - 0.373779f) > FLT_EPSILON)
    {
        printf("%f\n", mesh.tex_coords[mesh.n_vertices].values[0]);
        test_fail();
        // return;
    }

    if (fabs(mesh.tex_coords[mesh.n_vertices].values[1] - 0.770508) > FLT_EPSILON)
    {
        printf("%f\n", mesh.tex_coords[mesh.n_vertices].values[1]);
        test_fail();
        return;
    }
    */

    test_pass();
}

int main(void)
{
    window_init(100.0f, 100.0f, "test");
    mesh_load(&mesh, "assets/models/cyborg/", "cyborg");

    tex_coords_test();
    return 0;
}
