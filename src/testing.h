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

#ifndef TESTING_H
#define TESTING_H

#include <stdio.h>
#include <stdbool.h>
#include <float.h>

#ifdef _WIN32
#define test_start() printf("TESTING %s ", __func__)

#define test_pass() printf("SUCCESS\n")

#define test_fail() printf("FAILURE (line: %d)\n", __LINE__)

#else
#define test_start() printf("\033[0;34m"); \
                     printf("TESTING"); \
                     printf("\033[0m"); \
                     printf(" %s ", __func__)

#define test_pass() printf("\033[0;32m"); \
                    printf("SUCCESS\n"); \
                    printf("\033[0m")

#define test_fail() printf("\033[0;31m"); \
                    printf("FAILURE (line: %d)\n", __LINE__); \
                    printf("\033[0m")

#endif

#endif // TESTING_H
