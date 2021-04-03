cmake_minimum_required(VERSION 3.10)
project(jrCollection)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/deps/build/lib)

set(GAME_SOURCES
  src/core/backends/graphics_opengl.cpp
  src/core/backends/graphics_vulkan.cpp
  src/core/audio.cpp
  src/core/glad.c
  src/core/graphics.cpp
  src/core/input.cpp
  src/core/linear_algebra.cpp
  src/core/resource.cpp
  src/core/screen.cpp
  src/core/state.cpp
  src/core/util.cpp

  src/launcher/game_select.cpp
  src/launcher/main.cpp
  src/launcher/title.cpp

  src/img_editor/img_editor.cc

  src/z2d/z2d.cc
)

if(WIN32)
  list(APPEND GAME_SOURCES
    src/core/backends/audio_wasapi.cpp
  )
elseif(UNIX AND NOT APPLE)
  list(APPEND GAME_SOURCES
    src/core/backends/audio_pulseaudio.cpp)
endif()

add_executable(jrCollection ${GAME_SOURCES})
target_link_directories(jrCollection
  PUBLIC deps/build/lib
)
target_include_directories(jrCollection
  PUBLIC deps/build/include
  PUBLIC deps/header_only/include
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
  src/core/linear_algebra.cpp
  src/core/raster.cpp
  src/core/resource_importer.cpp
  src/core/resource.cpp
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
  PUBLIC deps/header_only/include
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
    png
    harfbuzz
    bz2
    brotlidec
    freetype
    vorbis
    vorbisenc
    ogg
    z
    samplerate
    assimp
  )
endif()

add_executable(config
  src/core/config.cpp
)
target_include_directories(config
  PUBLIC include
  PUBLIC deps/header_only/include
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
