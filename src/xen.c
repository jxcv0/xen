#define XEN_DEBUG
#include "xen.h"

#define SCREEN_W 800
#define SCREEN_H 600

#include <stdio.h>

int main()
{
    window_init(SCREEN_W, SCREEN_H, "KV");

    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);

    // projection matrix
    mat4_t p_mat = perspective(55.0f, 0.1f, 100.0f, ((float)SCREEN_H/(float)SCREEN_H));
    shader_set_uniform4fm(shader, "perspective", p_mat.values);
    
    // view matrix
    mat4_t v_mat = camera_view_matrix();
    shader_set_uniform4fm(shader, "view", v_mat.values);

    mesh_t mesh;
    load_mesh_obj(&mesh, "assets/models/cyborg/", "cyborg");

    // model matrix
    vec3_t v = construct_vec3(0.0f, 0.0f, 0.0f);
    vec3_t up = construct_vec3(0.0f, 1.0f, 0.0f);
    mat4_t m_mat = construct_mat4(1.0f);
    m_mat = translate(m_mat, v);
    m_mat = rotate(m_mat, up, 0.0f);

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
