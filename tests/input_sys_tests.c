#include "input_sys.h"
#include <assert.h>

int main()
{
	input_clear_buffer();
	assert(input_key_pressed(NONE));
	
	return 0;
}
