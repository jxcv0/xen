#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>
#include <float.h>
#include <assert.h>

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
