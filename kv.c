#include "xen.h"

bool run = true;

int main()
{
    init_window(800, 600, "KV");

    while(!should_close())
    {
        temp_esc();
        swap_buffers();
        poll_events();
    }

    close_window();

    return 0;
}
