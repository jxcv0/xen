#include "xen.h"
#include "testing.h"

// mesh_t mesh;

void tex_coords_test()
{
    test_start();

    // DELETED
    // asImport does some voodoo

    test_pass();
}

int main(void)
{
    xen_init();
    // mesh_load(&mesh, "assets/models/cyborg/", "cyborg");

    tex_coords_test();
    return 0;
}
