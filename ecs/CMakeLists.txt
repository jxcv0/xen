set(SOURCES entity_system.h entity_system.c)
add_library(ecs SHARED ${SOURCES})
target_include_directories(ecs PUBLIC .)
set_target_properties(ecs PROPERTIES LINKER_LANGUAGE C)

# tests
add_executable(entity_system_tests tests/entity_system_tests.c)
target_link_libraries(entity_system_tests PRIVATE ecs)
