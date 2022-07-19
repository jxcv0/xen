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
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct io_request
{
	char filepath[64];
	void *buffer;
	size_t nbytes;
};

#define IO_REQUEST_BUFFER_SIZE 64
static struct io_request io_request_buffer[IO_REQUEST_BUFFER_SIZE];
static int io_index;
static pthread_t io_thread;
static pthread_mutex_t io_mutex;
static pthread_cond_t io_cond;
static int io_run;

static void* io_routine(void* arg)
{
	while(io_run)
	{
		pthread_mutex_lock(&io_mutex);
		while (io_index == 0)
		{
			pthread_cond_wait(&io_cond, &io_mutex);
		}
		struct io_request ior = io_request_buffer[--io_index];
		pthread_mutex_unlock(&io_mutex);
		pthread_cond_signal(&io_cond);
		int fd = open(ior.filepath, O_RDONLY);
		assert(fd != -1);
		ssize_t n_read = read(fd, ior.buffer, ior.nbytes);
		assert(n_read != -1);
		close(fd);
	}
	return NULL;
}

// initialize resource system
int io_init(void)
{
	io_run = 1;
	io_index = 0;
	return pthread_create(&io_thread, NULL, io_routine, NULL);
}

#include <stdio.h>

// shut down resource system
void io_shutdown(void)
{
	io_run = 0;
	io_index = 1; // this is dubious
	pthread_cond_signal(&io_cond);
	pthread_join(io_thread, NULL);
}

// request a file be loaded into a buffer
int io_request(const char* filepath, size_t pathlen, void* buffer, size_t nbytes)
{
	if (pathlen > 64) { return -1; }
	pthread_mutex_lock(&io_mutex);
	if (io_index == IO_REQUEST_BUFFER_SIZE - 1) { return -1; }
	struct io_request *ior = &io_request_buffer[io_index++];
	strncpy(ior->filepath, filepath, pathlen);
	ior->buffer = buffer;
	ior->nbytes = nbytes;
	pthread_mutex_unlock(&io_mutex);
	pthread_cond_signal(&io_cond);
	return 0;
}

// wait for the request buffer to be empty
void io_wait(void)
{
	pthread_mutex_lock(&io_mutex);
	while (io_index != 0)
	{
		pthread_cond_wait(&io_cond, &io_mutex);
	}
	pthread_mutex_unlock(&io_mutex);
}
