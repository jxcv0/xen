/*
 * Copyright © 2022 Jacob Micoud
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include "resources.h"

#include "logger.h"

#include <pthread.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>

static bool IO_RUNNING;

// async
static pthread_t io_thread;
static pthread_mutex_t io_mutex;
static pthread_cond_t io_cond;

// io request type
struct io_request_t
{
	int fd;		// file descriptor
	int status;	// the status of the request
	void* buffer;	// buffer to write to
	size_t nbytes;	// size of buffer
	io_request_t *next;
};

static io_request_t *io_buffer_head;
static size_t io_buffer_len;

// the io_thread loop
static void* io_routine(void* arg)
{
	while(IO_RUNNING == true)
	{
		pthread_mutex_lock(&io_mutex);
		if (io_buffer_len == 0)
		{
			pthread_cond_wait(&io_cond, &io_mutex);
		}
		// get a request

		pthread_mutex_unlock(&io_mutex);
		// read file

	}
	return NULL;
}

// initialize resource system
void io_init(void)
{
	io_buffer_head = NULL;
	io_buffer_len = 0;
	if (pthread_create(&io_thread, NULL, io_routine, NULL) != 0)
	{
		xen_fail("Unable to create io_thread\n");
	}
}

// add a request to the request list and get a handle to it
io_request_t* io_request_file(const char* filepath, void *buffer, size_t buffer_size)
{
	io_request_t *ior = (io_request_t*)malloc(sizeof(io_request_t)); // TODO preallocate these
	pthread_mutex_lock(&io_mutex);
	ior->next = io_buffer_head;
	io_buffer_head = ior;
	io_buffer_len++;
}
