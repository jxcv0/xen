#define XEN_DEBUG
#include "xen.h"

#define SCREEN_W 800
#define SCREEN_H 600

int main()
{
    window_init(SCREEN_W, SCREEN_H, "KV");

    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);

    mat4_t p_mat = create_perspective_matrix(55.0f, 0.1f, 100.0f, SCREEN_H, SCREEN_H);
    shader_set_uniform4fm(shader, "perspective", p_mat.values);
    
    mat4_t v_mat = camera_view_matrix();
    shader_set_uniform4fm(shader, "view", v_mat.values);

    mesh_t mesh;
    load_mesh_obj(&mesh, "assets/models/cyborg/", "cyborg");

    float v[] = {0.0f, 0.0f, 0.0f};
    float up[] = {0.0f, 1.0f, 0.0f};
    mat4_t m_mat = create_translation_matrix(v);
    m_mat = rotate_m4(m_mat.values, up, 0.0f);
    shader_set_uniform4fm(shader, "model", m_mat.values);

    light_t light = create_default_light();
    shader_set_uniform3fv(shader, "light.color", light.color.values);
    shader_set_uniform3fv(shader, "light.position", light.position.values);
    shader_set_uniformf(shader, "light.constant", light.constant);
    shader_set_uniformf(shader, "light.linear", light.linear);
    shader_set_uniformf(shader, "light.quadratic", light.quadratic);

    checkerr();

    while(!should_close())
    {
        clear_buffers();

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
