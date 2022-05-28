#include "xen.h"

bool run = true;

int main()
{
    init_window(800, 600, "KV");

    // uber shader
    use_shader(load_shader("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag"));

    load_model("assets/models/ucube/ucube.obj");

    while(!should_close())
    {
        handle_input();
        swap_buffers();
        poll_events();
    }

    close_window();

    return 0;
}
