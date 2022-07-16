#include "resources.h"
#include "logger.h"

#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

int main()
{
	io_init();
	char buffer[32];
	io_request_t *ior = io_request("tests/data/aio.txt", buffer, 32);
	assert(ior);
	int result = io_wait(ior);
	printf("result: %d\n", result); //
	assert(result == IO_SUCCESS);
	assert(strcmp(buffer, "Measure twice cut once."));
	// TODO error handling
	xen_log("resources_tests passed");
	io_shutdown();
	return 0;
}
