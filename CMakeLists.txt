cmake_minimum_required(VERSION 3.10)
project(jrCollection)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/build/lib)
set(freetype_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deps/build/lib/cmake/freetype)
find_package(Freetype REQUIRED)

set(GAME_SOURCES
  src/backends/graphics_opengl.cpp
  src/backends/graphics_vulkan.cpp

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
elseif(UNIX AND NOT APPLE)
  list(APPEND GAME_SOURCES
    src/backends/audio_pulseaudio.cpp)
endif()

add_executable(jrCollection ${GAME_SOURCES})
target_link_directories(jrCollection
  PUBLIC deps/build/lib
)
target_include_directories(jrCollection
  PUBLIC deps/build/include
)
if(WIN32)
  target_link_libraries(jrCollection
    glfw
    zlibstaticd
    ogg
    vorbis
  )
else()
  target_link_libraries(jrCollection
    dl
    glfw
    z
    vorbis
    ogg
    pthread
  )
endif()

add_custom_command(TARGET jrCollection POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:jrCollection>/resources/"
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "resources/processed"
    "$<TARGET_FILE_DIR:jrCollection>/resources"
)

target_compile_definitions(jrCollection
  PUBLIC GAME
)

target_include_directories(jrCollection
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
    ${FREETYPE_LIBRARIES}
    vorbis
    vorbisenc
    ogg
    z
    samplerate
    assimp
  )
endif()

add_executable(config
  src/config.cpp
)
target_include_directories(config
  PUBLIC include
)
if(UNIX AND NOT APPLE)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  target_include_directories(config
    PUBLIC ${GTK3_INCLUDE_DIRS}
  )
  target_link_libraries(config
    ${GTK3_LIBRARIES}
  )
endif()
