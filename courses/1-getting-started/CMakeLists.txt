# library includes
include_directories(../../3rd/glm)
include_directories(../../3rd/glad/include)
include_directories(../../3rd/stb_image)
include_directories(../../3rd/glfw/include)

# executable
include_directories(include)
aux_source_directory(src SOURCES)
add_executable(1-getting-started ${SOURCES})
target_link_libraries(1-getting-started glfw glad)

# resource copy
add_custom_command(
    TARGET 1-getting-started PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}
)
