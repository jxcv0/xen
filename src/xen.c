#define XEN_DEBUG
#include "xen.h"

#define SCREEN_W 800
#define SCREEN_H 600

#include <stdio.h>

#include "gltestinl.h"
/*
int main()
{
    // window
    window_init(SCREEN_W, SCREEN_H, "KV");

    // shader
    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);
    
    // mesh
    mesh_t mesh;
    load_mesh_obj(&mesh, "assets/models/cyborg/", "cyborg");
    checkerr();

    while(!window_should_close())
    {
        clear_buffers();
        handle_input();

        // view matrix
        mat4_t v_mat = camera_view_matrix();
        shader_set_uniform(shader, "view", v_mat);
        shader_set_uniform(shader, "view_pos", camera_pos);

        // projection matrix
        mat4_t p_mat = perspective(180.0f, 0.1f, 100.0f, ((float)SCREEN_H/(float)SCREEN_W));
        shader_set_uniform(shader, "perspective", p_mat);
        
        // model matrix
        vec3_t mesh_pos = construct_vec3(0.0f, 0.0f, 0.0f);
        vec3_t up = construct_vec3(0.0f, 1.0f, 0.0f);
        mat4_t m_mat = construct_mat4(1.0f);
        m_mat = translate(m_mat, mesh_pos);
        m_mat = rotate(m_mat, up, 0.0f);
        shader_set_uniform(shader, "model", m_mat);

        // light
        light_t light = create_default_light();
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
*/
