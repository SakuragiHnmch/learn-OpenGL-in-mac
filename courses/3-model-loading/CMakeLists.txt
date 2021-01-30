# library includes
include_directories(../../3rd/glm)
include_directories(../../3rd/glad/include)
include_directories(../../3rd/stb_image)
include_directories(../../3rd/glfw/include)
include_directories(../../3rd/assimp/include)

# executable
include_directories(include)
aux_source_directory(src SOURCES)
add_executable(3-model-loading ${SOURCES})
target_link_libraries(3-model-loading glfw glad assimp)

# resource copy
add_custom_command(
    TARGET 3-model-loading
    COMMAND ${CMAKE_COMMAND} -E
    copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resource ${CMAKE_CURRENT_BINARY_DIR}
)
