cmake_minimum_required(VERSION 3.10)
project(fp2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fp2d
  src/glad.c
  src/graphics.cpp
  src/linear_algebra.cpp
  src/main.cpp

  src/imgui/imgui_impl_glfw.cpp
  src/imgui/imgui_impl_opengl3.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui_tables.cpp
  src/imgui/imgui_widgets.cpp
  src/imgui/imgui.cpp
)

target_include_directories(fp2d
  PUBLIC include
  PUBLIC include/imgui
  PUBLIC ${GLFW_PATH}/include
)

target_link_directories(fp2d
  PUBLIC ${GLFW_PATH}/lib
)

if(WIN32)
target_link_libraries(fp2d
  glfw3
)
endif()

if(APPLE)
target_include_directories(fp2d
  PUBLIC /usr/local/include
)

target_link_directories(fp2d
  PUBLIC /usr/local/lib
)

target_link_libraries(fp2d
  glfw
)
endif()
