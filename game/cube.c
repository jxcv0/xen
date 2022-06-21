#include "xen.h"

// main setup and game loop
int main()
{
    // window
    xen_init();
    camera_dir_init();

    // shader
    unsigned int shader = shader_load("assets/shaders/cube_uber.vert", "assets/shaders/cube_uber.frag");
    shader_use(shader);
    
    // mesh
    mesh_t mesh;
    mesh_load_simple(&mesh, "assets/models/ucube/ucube.obj");
    checkerr();

    // projection matrix
    mat4_t p_mat = perspective(55.0f, 0.1f, 100.0f, get_aspect());
    shader_set_uniform(shader, "projection", p_mat);
    checkerr();

    light_t light = create_default_light();
    light.position = construct_vec3(2.0f, 2.1f, 0.0f);
    light.color = construct_vec3(1.0f, 1.0f, 1.0f);
    light_t other_light = create_default_light();
    other_light.position = construct_vec3(-2.0f, 3.0f, 1.0f);
    other_light.color = construct_vec3(1.0f, 0.0f, 0.0f);

    float last_frame = 0.0f;

    // TODO will be producer loop
    while(!window_should_close())
    {
        float current_frame = (float)glfwGetTime();
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        handle_input(delta_time);

        // view matrix
        shader_set_uniform(shader, "view", camera_view_matrix());
        shader_set_uniform(shader, "view_pos", get_camera_pos());

        // model matrix
        vec3_t col = construct_vec3(0.0f, 0.3f, 0.3f);
        shader_set_uniform(shader, "model", mesh_model_matrix(&mesh));
        shader_set_uniform(shader, "base_col", col);

        // light
        shader_set_uniform(shader, 0, &light);
        checkerr();
        shader_set_uniform(shader, 1, &other_light);
        shader_set_uniform(shader, "shininess", 32.0f);

        // draw
        clear_buffers();
        draw_mesh_simple(&mesh, shader);

        swap_buffers();
        poll_events();
        checkerr();
    }

    free_mesh(&mesh);
    close_window();

    return 0;
}
