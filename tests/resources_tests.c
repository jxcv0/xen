#include "resources.h"
#include "logger.h"

#include <assert.h>

int main()
{
	io_init();
	
	char buffer[1028];
	io_request_t *ior = io_request("tests/data/aio.txt", buffer, 1028);
	assert(ior);
	io_wait(ior);

	xen_log("resources_tests passed\n");
	io_shutdown();
	return 0;
}
