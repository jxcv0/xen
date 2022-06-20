/* 
 * Copyright (c) 2022 Jacob Micoud
 * Licensed under GPL-3.0-or-later
 */

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdlib.h>

// FIFO circular buffer
typedef struct ringbuffer_t
{
    void* buffer;
    size_t stride, max_size, readptr, writeptr;
} ringbuffer_t;

// set up the buffer data
ringbuffer_t construct_cbuff(size_t s, size_t n)
{
    ringbuffer_t b = {
        .stride = s,
        .max_size = n,
        .readptr = 0,
        .writeptr = 0,
        .buffer = malloc(s * n)
    };
    return b;
}

// add an item to the buffer
void write(ringbuffer_t* buffer, void* item)
{
    // TODO
}

// get the next item from the buffer
void* read(ringbuffer_t* buffer)
{
    // TODO
}

// get the number of items in the buffer
size_t size(const ringbuffer_t* b)
{
    // TODO
}

#endif // RING_BUFFER_H
