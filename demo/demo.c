#include "graphics.h"
#include "resources.h"
#include "input.h"
#include "xen.h"

int main(void)
{
	io_init();
	graphics_init();

	while (!graphics_window_should_close())
	{
		frame_begin();
		frame_end();
	}

	io_shutdown();
	graphics_shutdown();
	return 0;
}
