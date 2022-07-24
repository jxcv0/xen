#include "window.h"
#include "graphics.h"
#include "resources.h"
#include "input.h"
#include "xen.h"

int main(void)
{
	window_init();
	graphics_init();
	io_init();

	while (!window_should_close())
	{
		frame_begin();
		if (input_key_pressed(KEY_ESC)) {
			set_window_should_close(true);
		}
		frame_end();
	}

	window_close();
	io_shutdown();
	return 0;
}
