cmake_minimum_required(VERSION 3.10)
project(fp2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(fp2d
  src/imgui/imgui_impl_glfw.cpp
  src/imgui/imgui_impl_opengl3.cpp
  src/imgui/imgui_draw.cpp
  src/imgui/imgui_tables.cpp
  src/imgui/imgui_widgets.cpp
  src/imgui/imgui.cpp

  src/levels/training_levels.cpp

  src/screens/level_editor_screen.cpp
  src/screens/level_screen.cpp
  src/screens/options_screen.cpp
  src/screens/title_screen.cpp

  src/glad.c
  src/graphics.cpp
  src/input.cpp
  src/level.cpp
  src/linear_algebra.cpp
  src/main.cpp
  src/resource.cpp
  src/screen.cpp
  src/state.cpp
  src/util.cpp
)

add_custom_command(TARGET fp2d POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:fp2d>/resources/"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "resources/processed"
    "$<TARGET_FILE_DIR:fp2d>/resources"
)

target_compile_definitions(fp2d
  PUBLIC GAME
)

target_include_directories(fp2d
  PUBLIC include
  PUBLIC include/imgui
  PUBLIC ${GLFW_PATH}/include
  PUBLIC ${ZLIB_PATH}/include
)

target_link_directories(fp2d
  PUBLIC ${GLFW_PATH}/lib
  PUBLIC ${ZLIB_PATH}/lib
)

if(WIN32)
target_link_libraries(fp2d
  glfw3
  zlibstaticd
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
  z
)
endif()


if(BUILD_RESOURCE_IMPORTER)
add_executable(resource_importer
  src/resource_importer.cpp
  src/resource.cpp
)

target_compile_definitions(resource_importer
  PUBLIC RESOURCE_IMPORTER
  PUBLIC RESOURCE_IMPORT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)

target_include_directories(resource_importer
  PUBLIC include
  PUBLIC ${ZLIB_PATH}/include
  PUBLIC ${FT_PATH}/include
  PUBLIC ${FT_PATH}/include/freetype2
)

target_link_directories(resource_importer
  PUBLIC ${ZLIB_PATH}/lib
  PUBLIC ${FT_PATH}/lib
)

target_link_libraries(resource_importer
  zlibstaticd
  freetyped
)

if(APPLE)
target_include_directories(resource_importer
  PUBLIC /usr/local/include
  PUBLIC /usr/local/include/freetype2
)

target_link_directories(resource_importer
  PUBLIC /usr/local/lib
)
endif()
endif()
