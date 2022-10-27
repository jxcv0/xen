cmake_minimum_required(VERSION 3.16)
project(xen LANGUAGES C VERSION 0.1.0)

set(CMAKE_CXX_COMPILER /usr/bin/gcc)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_CREATE_SHARED_LIBRARY C)
add_compile_options("-Wno-dev -ggdb")

enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)

set(
  HEADERS 
    include/vec.h
    include/logger.h
    include/resources.h
    include/material.h
    include/texture.h
    include/model.h
    include/mesh.h
    include/vertex.h
)

set(
  SOURCES 
    src/resources.c
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C)

target_include_directories(
  ${PROJECT_NAME}
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/lib/stb
)

target_link_libraries(
  ${PROJECT_NAME}
    PUBLIC
      glfw
      glad
      OpenGL::GL
      dl
      assimp
      m
)

add_subdirectory(lib/glad)
add_subdirectory(example)
add_subdirectory(test)
add_subdirectory(ecs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

