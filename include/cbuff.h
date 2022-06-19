#ifndef CBUFF_H
#define CBUFF_H

#include <stdint.h>

// circular buffer
typedef struct cbuff_t
{
    void* buffer;
    size_t stride, max_size, readptr, writeptr;
} cbuff_t;

// add an item to the buffer
void write(cbuff_t* buffer, void* item);

#endif // CBUFF_H
