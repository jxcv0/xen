#define XEN_DEBUG
#include "xen.h"

#define SCREEN_W 800.0f
#define SCREEN_H 600.0f

#include <stdio.h>

// #include "gltestinl.h"
// /*

int main()
{
    // window
    window_init(SCREEN_W, SCREEN_H, "KV");

    // TODO remove
    camera_dir = normalize_vec3(construct_vec3(-0.7f, -0.2f, -0.7f));

    // shader
    unsigned int shader = shader_load("assets/shaders/ubershader.vert", "assets/shaders/ubershader.frag");
    shader_use(shader);
    
    // mesh
    mesh_t mesh;
    mesh_load(&mesh, "assets/models/cyborg/", "cyborg");
    checkerr();

    // projection matrix
    mat4_t p_mat = perspective(45.0f, 0.1f, 100.0f, (SCREEN_W/SCREEN_H));
    shader_set_uniform(shader, "projection", p_mat);
    checkerr();

    light_t light = create_default_light();

    float last_frame = 0.0f;

    // TODO will be producer loop
    while(!window_should_close())
    {
        float current_frame = (float)glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        handle_input(delta_time);
        clear_buffers();

        // view matrix
        mat4_t v_mat = camera_view_matrix();
        shader_set_uniform(shader, "view", v_mat);
        shader_set_uniform(shader, "view_pos", camera_pos);

        // model matrix
        vec3_t mesh_pos = construct_vec3(0.0f, 0.0f, 0.0f);
        vec3_t up = construct_vec3(0.0f, 1.0f, 0.0f);
        mat4_t m_mat = construct_mat4(1.0f);
        m_mat = translate(m_mat, mesh_pos);
        m_mat = rotate(m_mat, up, 0.0f);
        shader_set_uniform(shader, "model", m_mat);

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
// */
