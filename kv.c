#define XEN_DEBUG
#include "xen.h"

int main()
{
    init_window(800, 600, "KV");
    use_shader(load_shader("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag"));
    mesh_t mesh;
    load_mesh_obj(&mesh, "assets/models/cyborg/", "cyborg");

    while(!should_close())
    {
        draw_mesh(&mesh);
        handle_input();
        swap_buffers();
        poll_events();
    }

    free_mesh(&mesh);

    close_window();

    return 0;
}
