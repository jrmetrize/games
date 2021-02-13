cmake_minimum_required(VERSION 3.10)
project(fp2d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED PATHS deps/build)
find_package(Freetype REQUIRED PATHS deps/build)

set(GAME_SOURCES
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
  src/screens/splash_screen.cpp
  src/screens/title_screen.cpp

  src/audio.cpp
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

if(WIN32)
  list(APPEND GAME_SOURCES
    src/backends/audio_wasapi.cpp
  )
endif()

add_executable(fp2d ${GAME_SOURCES})
target_link_directories(fp2d
  PUBLIC deps/build/lib
)
target_include_directories(fp2d
  PUBLIC deps/build/include
)
if(WIN32)
  target_link_libraries(fp2d
    glfw
    zlibstaticd
  )
else()
  target_link_libraries(fp2d
    dl
    glfw
    z
  )
endif()

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
)

add_executable(resource_importer
  src/linear_algebra.cpp
  src/raster.cpp
  src/resource_importer.cpp
  src/resource.cpp
)

target_compile_definitions(resource_importer
  PUBLIC RESOURCE_IMPORTER
  PUBLIC RESOURCE_IMPORT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/resources/"
)

target_include_directories(resource_importer
  PUBLIC include
  PUBLIC deps/build/include/freetype2
)

target_link_directories(resource_importer
  PUBLIC deps/build/lib
)
target_include_directories(resource_importer
  PUBLIC deps/build/include
)
if(WIN32)
  target_link_libraries(resource_importer
    freetyped
    zlibstaticd
    samplerate
    ogg
    vorbis
  )
else()
  target_link_libraries(resource_importer
    freetype
    ogg
    vorbis
    z
  )
endif()
