cmake_minimum_required(VERSION 3.16)
project(xentests LANGUAGES C)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(TEST_COMMON_LIBS xen)

add_executable(resources_tests resources_tests.c)
add_executable(logger_tests logger_tests.c)

target_link_libraries(resources_tests ${TEST_COMMON_LIBS})
target_link_libraries(logger_tests ${TEST_COMMON_LIBS})
