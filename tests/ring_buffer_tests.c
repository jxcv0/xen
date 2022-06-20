#include "ring_buffer.h"
#include "assert.h"

void cbuff_init_test()
{
    ringbuffer_t int_buffer = construct_cbuff(sizeof(int), 10);
    assert(int_buffer.stride == sizeof(int));
    assert(int_buffer.max_size == sizeof(int) * 10);
    assert(int_buffer.readptr == 0);
    assert(int_buffer.writeptr == 0);
}

int main(void)
{
    cbuff_init_test();
    return 0;
}
