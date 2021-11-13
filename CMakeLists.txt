#
# Copyright 2021 Collabora, Ltd.
#
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.10)

project(native-example-bundle)

set(SOURCES
  src/car.cpp
  src/font.cpp
  src/hiscore.cpp
  src/level.cpp
  src/main.cpp
  src/model.cpp
  src/race.cpp
  src/render.cpp
  src/title_screen.cpp
  src/players/ai_player.cpp
  src/players/human_player.cpp
  src/players/joystick_player.cpp
  src/players/modern_pad_player.cpp
  src/players/pad_player.cpp
)

set(RESOURCES
  res/bash.wav
  res/beep.wav
  res/crash.wav
  res/track.dat
  res/CourierPrime-Regular.ttf
  res/OFL.txt
)

if(POLICY CMP0072)
  # Prefer the old style libGL.so over the new style libOpenGL.so, for now
  cmake_policy(SET CMP0072 OLD)
endif()

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(native ${SOURCES})
target_compile_options(native PRIVATE -Wall -Wextra -flto -O3 -pedantic --std=c++17 -g -ggdb)
target_link_options(native PRIVATE -g -ggdb)
target_include_directories(native PRIVATE
  ${CMAKE_SOURCE_DIR}/src
  ${OPENGL_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIRS}
)
target_include_directories(native PRIVATE SYSTEM
  lib/controllers
)
target_link_libraries(native PRIVATE
  ${SDL2_LIBRARIES}
  ${OPENGL_LIBRARIES}
  SDL2_ttf
  SDL2_mixer
  stdc++fs
)

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_NAME "native-indy800-example")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}")

include(CPack)

install(TARGETS native
  RUNTIME DESTINATION bin
)
install(FILES ${RESOURCES} DESTINATION res)
install(FILES bundle.ini DESTINATION .)
install(PROGRAMS launch-game.sh DESTINATION .)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib DESTINATION .)
