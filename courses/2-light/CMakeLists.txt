# executable
aux_source_directory(src SOURCES)
add_executable(2-light ${SOURCES})
target_link_libraries(2-light glfw glad)
