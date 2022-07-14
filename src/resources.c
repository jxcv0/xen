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

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <signal.h>

#define IO_SIGNAL SIGUSR1   // Signal used to notify I/O completion

// cancel all requests on reciept of SIGQUIT
static sig_atomic_t RECIEVED_SIGQUIT = 0;

// passed to sigaction
static void handle_quit(int sig) { RECIEVED_SIGQUIT = 1; }

// passed to sigaction
static void handle_io_signal(int sig, siginfo_t *si, void *uctx)
{
	if (si->si_code == SI_ASYNCIO)
	{
		xen_log("I/O completion signal recieved\n");
		// struct ioRequest *ioReq = si->si_value.sival_ptr;
		// int fd = ioReq->aiocbp->aio_fildes;
	}
	// TODO other signals?
}

// node of io_request linked list array
struct io_request
{
	struct io_request *next;
	int reqnum;
	int status;
	struct aiocb ctl_block;
};

// linked list of IO requests
static struct
{
	struct io_request *head;
	int size;
} io_req_list;

// signal handling
static struct sigaction resources_sa;

// initialize the resource system
void xen_resources_init()
{
	// request list
	io_req_list.head = NULL;
	io_req_list.size = 0;

	// SIGQUIT
	resources_sa.sa_flags = SA_RESTART;
	sigemptyset(&resources_sa.sa_mask);
	resources_sa.sa_handler = handle_quit;
	if (sigaction(SIGQUIT, &resources_sa, NULL) == -1)
	{
		xen_fail("Unable to set SIGQUIT handler\n");
	}
	
	// IO_SIGNAL
	resources_sa.sa_flags = SA_RESTART | SA_SIGINFO;
	resources_sa.sa_sigaction = handle_io_signal;
	if (sigaction(IO_SIGNAL, &resources_sa, NULL) == -1)
	{
		xen_fail("Unable to set IO_SIGNAL handler\n");
	}
}

// add a request to the IO request list
int xen_request_io(const char* filename)
{
	// TODO move sigaction stuff to xen_resources_init();
	// struct io_request *ior = (struct io_request*)malloc(sizeof(struct io_request));
	
	// if (ior == NULL) { xen_fail("Unable to allocate memory for I/O request\n"); }
	return 0;
}
