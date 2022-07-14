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
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#define IO_REQUEST_BUFFER_SIZE 64

static bool IO_RUNNING;

// async
static pthread_t io_thread;
static pthread_mutex_t io_mutex;
static pthread_cond_t io_cond;

// io request buffer
struct io_request
{
	int fd;		// file descriptor
	int status;	// the status of the request
	void* buffer;	// buffer to write to
	size_t nbytes;	// size of buffer
};
static struct io_request io_request_buffer[IO_REQUEST_BUFFER_SIZE];

static int io_index = 0;

// check if the buffer is empty
static inline bool io_buffer_empty(void)
{
	return io_index == 0;
}

// check if the buffer is full
static inline bool io_buffer_full(void)
{
	return io_index == IO_REQUEST_BUFFER_SIZE - 1;
}

// add an entry to the buffer and advance io_index
static inline int io_buffer_push(struct io_request ior)
{
	if (!io_buffer_full())
	{
		io_request_buffer[io_index++] = ior;
		return 0;
	}
	else
	{
		return IO_ERR_BUFFER_FULL;
	}
}

// get an entry from the buffer and decrement io_index
static inline struct io_request io_buffer_pop(void)
{
	assert(io_index > 0);
	return io_request_buffer[io_index--];
}

// open and read a file specified by a request
// set status flag as appropriate
static inline void io_execute_request(struct io_request *req)
{
	// TODO
}

// the async IO loop
static void* io_routine(void* arg)
{
	while (IO_RUNNING)
	{
		pthread_mutex_lock(&io_mutex);
		if (io_buffer_empty())
		{
			pthread_cond_wait(&io_cond, &io_mutex);
		}
		struct io_request ior = io_buffer_pop();
		pthread_mutex_unlock(&io_mutex);
		io_execute_request(&ior);
	}
	return NULL;
}

// initialize resource system
void io_init(void)
{
	xen_log("Starting resource manager\n");
	int e = pthread_create(&io_thread, NULL, io_routine, NULL);
	if (e != 0)
	{
		printf("pthread err: %d\n", e);
		xen_fail("Unable to create io_thread\n");
	}
}

// shut down resource system
void io_shutdown(void)
{
	IO_RUNNING = false;
	pthread_join(io_thread, NULL);
}

// add a request to the IO request list and get a handle to it
int io_make_request(const char* filepath, void *buffer, size_t nbytes)
{
	int fd = open(filepath, O_RDONLY);
	if (fd == -1)
	{
		printf("ERRNO: %d\n", errno);
		xen_log("Unable to open file\n");
		return -1;
	}

	struct io_request r = {
		.fd = fd,
		.status = IO_INPROGRESS,
		.nbytes = nbytes
	};
	
	return io_buffer_push(r);
}

// wait on async io request
void io_wait(int request_handle)
{

}
