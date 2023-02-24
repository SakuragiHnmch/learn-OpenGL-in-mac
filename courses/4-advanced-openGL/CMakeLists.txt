# executable
aux_source_directory(src SOURCES)
add_executable(4-advanced-openGL ${SOURCES})
target_link_libraries(4-advanced-openGL glfw glad assimp)