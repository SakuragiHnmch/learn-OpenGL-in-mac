# GLFW
add_subdirectory(glfw)

# GLM
include_directories(glm)

# GLAD
include_directories(glad/include)
add_library(glad glad/src/glad.c)

# STB Image
include_directories(stb_image)

# Assimp
include_directories(assimp/include)
add_subdirectory(assimp)
