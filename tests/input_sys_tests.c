#include "input_sys.h"

#include <stdio.h>
#include <assert.h>

int main()
{
	input_clear_buffer();
	assert(input_key_pressed(KEY_NONE));
	input_set_buffer(0x00000001);
	assert(input_key_pressed(KEY_W));

	printf("input_sys_tests passed\n");
	
	return 0;
}