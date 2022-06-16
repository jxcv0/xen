#define XEN_DEBUG
#include "xen.h"

#include <stdio.h>

int main()
{
    // window
    xen_init();

    // shader
    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);
    
    // mesh
    mesh_t mesh;
    mesh_load(&mesh, "assets/models/cyborg/", "cyborg");
    checkerr();

    // projection matrix
    mat4_t p_mat = perspective(55.0f, 0.1f, 100.0f, (screen_w/screen_h));
    shader_set_uniform(shader, "projection", p_mat);
    checkerr();

    light_t light = create_default_light();

    float last_frame = 0.0f;
    
    camera_update_dir(window, (double)screen_w/2.0, (double)screen_h/2.0);

    // TODO will be producer loop
    while(!window_should_close())
    {
        float current_frame = (float)glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        handle_input(delta_time);
        clear_buffers();

        // view matrix
        shader_set_uniform(shader, "view", camera_view_matrix());
        shader_set_uniform(shader, "view_pos", camera_pos);

        // model matrix
        shader_set_uniform(shader, "model", mesh_model_matrix(&mesh));

        // light
        shader_set_uniform(shader, "light.color", light.color);
        shader_set_uniform(shader, "light.position", light.position);
        shader_set_uniform(shader, "light.constant", light.constant);
        shader_set_uniform(shader, "light.linear", light.linear);
        shader_set_uniform(shader, "light.quadratic", light.quadratic);
        shader_set_uniform(shader, "shininess", 32.0f);

        // draw
        draw_mesh(&mesh, shader);

        swap_buffers();
        poll_events();
        checkerr();
    }

    free_mesh(&mesh);
    close_window();

    return 0;
}
