# library includes
include_directories(../../3rd/glm)
include_directories(../../3rd/glad/include)
include_directories(../../3rd/stb_image)
include_directories(../../3rd/glfw/include)

# executable
include_directories(include)
aux_source_directory(src SOURCES)
add_executable(2-light ${SOURCES})
target_link_libraries(2-light glfw glad)

# resource copy
add_custom_command(
    TARGET 2-light PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}
)
