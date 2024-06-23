cmake_minimum_required(VERSION 3.5)

project("glfw_shader")

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC input.cpp render.cpp shader.cpp display.cpp main.cpp)
  
add_executable(glfw_shader ${SRC} )

target_include_directories(glfw_shader BEFORE PUBLIC /usr/include/GLFW)
target_link_libraries(glfw_shader X11 GL GLEW /usr/lib/x86_64-linux-gnu/libglfw.so.3.3)

#target_include_directories(playfield BEFORE PUBLIC /usr/include)





