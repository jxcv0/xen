#include "resources.h"
#include "logger.h"

#include <assert.h>

int main()
{
	io_init();

	xen_log("resources_tests passed\n");
	return 0;
}
