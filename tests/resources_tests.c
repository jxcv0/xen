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
#include "maths.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char buffer[32];

void io_sys_test(void)
{
	memset(&buffer, 0, 32);
	assert(io_init() == 0);
	const char* filepath = "tests/data/aio.txt";
	assert(io_request(filepath, strlen(filepath), buffer, 32) == 0);
	assert(io_request(filepath, strlen(filepath), buffer, 32) == 0);
	assert(io_request(filepath, strlen(filepath), buffer, 32) == 0);
	assert(io_request(filepath, strlen(filepath), buffer, 32) == 0);
	assert(io_request(filepath, strlen(filepath), buffer, 32) == 0);
	io_wait();
	assert(strcmp(buffer, "Measure twice cut once.\n") == 0);
	io_shutdown();

	xen_log("aio test passed.");
}

void io_load_mesh_test(void)
{
	mesh_t mesh;
	io_load_mesh(&mesh, "assets/test/test_obj.obj");
	assert(fabs(mesh.vertices[1].values[0] - (-0.543903)) < FLT_EPSILON);
	assert(fabs(mesh.vertices[1].values[1] - 0.838724) < FLT_EPSILON);
	assert(fabs(mesh.vertices[1].values[2] - 0.920782) < FLT_EPSILON);
	assert(fabs(mesh.texcoords[1].values[0] - 0.875000) < FLT_EPSILON);
	assert(fabs(mesh.texcoords[1].values[1] - 0.750000) < FLT_EPSILON);
	assert(fabs(mesh.normals[1].values[0] - 0.0757) < FLT_EPSILON);
	assert(fabs(mesh.normals[1].values[1] - 0.9938) < FLT_EPSILON);
	assert(fabs(mesh.normals[1].values[2] - 0.0817) < FLT_EPSILON);
	free_mesh(&mesh);
}

int main(void)
{
	io_sys_test();
	io_load_mesh_test();
	xen_log("resources tests passed");
	return 0;
}
