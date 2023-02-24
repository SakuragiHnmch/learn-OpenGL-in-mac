# executable
aux_source_directory(src SOURCES)
add_executable(5-tessellation ${SOURCES})
target_link_libraries(5-tessellation glfw glad)
