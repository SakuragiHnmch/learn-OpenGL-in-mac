# executable
aux_source_directory(src SOURCES)
add_executable(3-model-loading ${SOURCES})
target_link_libraries(3-model-loading glfw glad assimp)
