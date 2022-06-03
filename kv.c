#define XEN_DEBUG
#include "xen.h"

#define SCREEN_W 800
#define SCREEN_H 600

int main()
{
    init_window(SCREEN_W, SCREEN_H, "KV");

    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);

    mat4_t p_mat = create_perspective_matrix(55.0f, 0.1f, 100.0f, SCREEN_H, SCREEN_H);
    set_shader_uniform4fv(shader, "perspective", p_mat.values);

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
