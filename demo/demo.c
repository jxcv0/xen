#include "window.h"
#include "graphics.h"
#include "resources.h"
#include "input.h"
#include "xen.h"

#include <assert.h>
int main(void)
{
	window_init();
	graphics_init();

	mesh_t mesh;
	pthread_t thread;
	struct io_request ior;
	ior.filepath = "assets/test/test_obj.obj";
	ior.struct_ptr = &mesh;
	ior.thread_ptr = &thread;

	assert(io_load_mesh_async(&ior) != -1);
	pthread_join(thread, NULL);
	print_vec3(mesh.vertices[0]);

	while (!window_should_close())
	{
		frame_begin();
		if (input_key_pressed(KEY_ESC)) {
			set_window_should_close(true);
		}
		frame_end();
	}

	free_mesh(&mesh);
	window_close();
	return 0;
}
