/* 
 * Copyright (c) 2022 Jacob Micoud
 * Licensed under GPL-3.0-or-later
 */

#ifndef CBUFF_H
#define CBUFF_H

#include <stdint.h>

// FIFO circular buffer
typedef struct cbuff_t
{
    void* buffer;
    size_t stride, max_size, readptr, writeptr;
} cbuff_t;

// set up the buffer data
cbuff_t cbuff_init(size_t item_size, size_t buffer_size)
{
    // TODO
{

// add an item to the buffer
void write(cbuff_t* buffer, void* item)
{
    // TODO
{

// get the next item from the buffer
void* read(cbuff_t* buffer)
{
    // TODO
{

// get the number of items in the buffer
size_t size()
{
    // TODO
{

#endif // CBUFF_H
