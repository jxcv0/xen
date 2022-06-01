#define XEN_DEBUG
#include "xen.h"

int main()
{
    init_window(800, 600, "KV");

    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);

    mesh_t mesh;
    load_mesh_obj(&mesh, "assets/models/cyborg/", "cyborg");

    checkerr();

    while(!should_close())
    {
        draw_mesh(&mesh, shader);
        handle_input();
        swap_buffers();
        poll_events();
        checkerr();
    }

    free_mesh(&mesh);

    close_window();

    return 0;
}
