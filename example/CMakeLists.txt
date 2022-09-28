cmake_minimum_required(VERSION 3.16)
project(xengineexample LANGUAGES C VERSION 0.1.0)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_executable(example main.c)

target_link_libraries(example xen)

