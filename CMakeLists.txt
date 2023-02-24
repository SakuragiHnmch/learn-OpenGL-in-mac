cmake_minimum_required(VERSION 3.17)
project(LearnOpenGLMac)

set(CMAKE_CXX_STANDARD 14)

include_directories(3rd/glm)
include_directories(3rd/glad/include)
include_directories(3rd/stb_image)
include_directories(3rd/glfw/include)
include_directories(3rd/assimp/include)
include_directories(common/include)

# 3rd library
add_subdirectory(3rd)

# courses
add_subdirectory(courses)
