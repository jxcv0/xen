#include "window.h"
#include "graphics.h"
#include "shader.h"
#include "resources.h"
#include "input.h"
#include "camera.h"
#include "xen.h"

int main(void)
{
	// init
	window_init();
	graphics_init();


	unsigned int ubershader = shader_load("assets/shaders/ubershader.vert",
					  "assets/shaders/ubershader.frag");

	mat4_t projection_matrix = window_perspective_matrix(55.0f);
	shader_set_uniform(ubershader, "projection", projection_matrix);

	// load 3d model
	mesh_t mesh;
	pthread_t thread;
	struct io_request ior;
	ior.filepath = "assets/test/test_obj.obj";
	ior.struct_ptr = &mesh;
	ior.thread_ptr = &thread;

	set_camera_update_callback(camera_update_dir);
	if (io_load_mesh_async(&ior) == -1) {
		perror("io_load_mesh_async");
	}
	pthread_join(thread, NULL);
	gen_buffer_objects(&mesh);

	while (!window_should_close())
	{
		frame_begin();
		if (input_key_pressed(KEY_ESC)) {
			set_window_should_close(true);
		}
		draw_mesh(ubershader, &mesh);
		frame_end();
		checkerr();
	}

	free_mesh(&mesh);
	window_close();
	return 0;
}
