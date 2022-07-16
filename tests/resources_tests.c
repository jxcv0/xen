#include "resources.h"
#include "logger.h"

#include <assert.h>
#include <string.h>

int main()
{
	io_init();
	char buffer[32];
	io_request_t *ior = io_request("tests/data/aio.txt", buffer, strlen(buffer));
	assert(ior);
	int result = io_wait(ior);
	assert(result == IO_SUCCESS);
	assert(strcmp(buffer, "Measure twice cut once.") == 0);
	io_shutdown();

	xen_log("resources_tests passed");
	return 0;
}
