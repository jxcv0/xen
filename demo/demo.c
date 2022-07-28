#include "window.h"
#include "graphics.h"
#include "shader.h"
#include "resources.h"
#include "input.h"
#include "camera.h"
#include "xen.h"

#include <stdlib.h>

int main(void)
{
	// init
	window_init();
	graphics_init();

	// load text file
	pthread_t vert_thread;
	char* vert_shader = NULL;
	struct io_request vert_ior = {
		.filepath = "assets/shaders/ubershader.vert",
		.struct_ptr = vert_shader,
		.thread_ptr = &vert_thread
	};
	io_load_text_async(&vert_ior);
	pthread_join(vert_thread, NULL);
	if (vert_shader == NULL) {
		perror("vert_shader is NULL");
	} else {
		printf("%s\n", vert_shader);
	}
	// compile and link
	free(vert_shader);

	// end load text file


	unsigned int ubershader = shader_load("assets/shaders/ubershader.vert",
					      "assets/shaders/ubershader.frag");

	mat4_t projection_matrix = window_perspective_matrix(55.0f);
	shader_set_uniform(ubershader, "projection", projection_matrix);
	checkerr();

	// load 3d model
	mesh_t mesh;
	pthread_t thread;
	struct io_request ior = {
		.filepath = "assets/test/test_obj.obj",
		.struct_ptr = &mesh,
		.thread_ptr = &thread
	};

	set_camera_update_callback(camera_update_dir); // do other work
	io_load_mesh_async(&ior);
	pthread_join(thread, NULL);
	// end load 3d model

	checkerr();
	gen_buffer_objects(&mesh);
	checkerr();

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
