# executable
aux_source_directory(src SOURCES)
add_executable(6-skeleton ${SOURCES})
target_link_libraries(6-skeleton glfw glad assimp)
