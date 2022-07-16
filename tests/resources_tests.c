#include "resources.h"
#include "logger.h"

#include <assert.h>
#include <stdio.h>
#include <pthread.h>

int main()
{
	io_init();
	char buffer[32];
	io_request_t *ior = io_request("tests/data/aio.txt", buffer, 32);
	assert(ior);
	int result = io_wait(ior);
	printf("result: %d\n", result);
	assert(result == IO_SUCCESS);
	xen_log("resources_tests passed\n");
	io_shutdown();
	return 0;
}
