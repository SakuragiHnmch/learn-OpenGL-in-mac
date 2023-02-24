# executable
aux_source_directory(src SOURCES)
add_executable(1-getting-started ${SOURCES})
target_link_libraries(1-getting-started glfw glad)
