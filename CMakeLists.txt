cmake_minimum_required(VERSION 3.10)
project(fp2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fp2d
  src/glad.c
  src/main.cpp
)

target_include_directories(fp2d
  PUBLIC include
  PUBLIC ${GLFW_PATH}/include
)

target_link_directories(fp2d
  PUBLIC ${GLFW_PATH}/lib
)

target_link_libraries(fp2d
  glfw3
)
