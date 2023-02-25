# executable
add_executable(5-cpu-tessellation src/cputessellation.cpp)
target_link_libraries(5-cpu-tessellation glfw glad)

add_executable(5-gpu-tessellation src/gputesselation.cpp)
target_link_libraries(5-gpu-tessellation glfw glad)
