# Required CMake version.
cmake_minimum_required(VERSION 2.8)

# Project name.
project(AaOpenGL C CXX)
set(CMAKE_CXX_FLAGS "/EHa")

# Module directory.
set(CMAKE_MODULE_PATH $ENV{TLR_DEV}/cmake-modules)
STRING(REGEX REPLACE "\\\\" "/" CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

# OpenGL dependency.
find_package(OpenGL REQUIRED) # GLEW

# glew dependency
set(GLEW_DIR $ENV{TLR_DEV}/glew-1.7.0)
find_package(GLEW REQUIRED)

# AaToolkit dependency
SET(AATOOLKIT_DIR $ENV{TLR_DEV}/aassif-toolkit)
find_package(AaToolkit REQUIRED)

# Include directoires.
include_directories(
  include
  #include/bits
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIR}
  ${AATOOLKIT_INCLUDE_DIR}
)

# Include files.
set(PROJECT_HDR
  include/AaExtensions
  include/AaGLFF
  include/AaGLSL
  include/AaGlutMenu
  include/AaGlutText
  include/AaGlutWatermark
  include/AaGrabber
  include/AaLight
  include/AaPrimitives
  include/AaReferenceFrame
  include/AaScene
  include/bits/AaGL.h
  include/bits/AaExtensions.h
  include/bits/AaGLFF.hh
  include/bits/AaGLSL.h
  include/bits/AaGlutMenu.hh
  include/bits/AaGlutText.hh
  include/bits/AaGlutWatermark.hh
  include/bits/AaGrabber.h
  include/bits/AaLight.h
  include/bits/AaPrimitives.h
  include/bits/AaReferenceFrame.hh
  include/bits/AaScene.h
)

# Source files.
set(PROJECT_SRC
  src/AaExtensions.cc
  src/AaGLSL.cc
  src/AaGrabber.cc
  src/AaLight.cc
  src/AaPrimitives.cc
  src/AaScene.cc
)

# Target generation.
add_library(AaOpenGL
  SHARED
  ${PROJECT_HDR}
  ${PROJECT_SRC}
)

# Output paths.
set_target_properties(AaOpenGL
  PROPERTIES
  #RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  #LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  #ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release
  LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/lib/debug
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/lib/debug
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release
  DEBUG_POSTFIX d
)

# Target dependencies.
target_link_libraries(AaOpenGL
  ${OPENGL_LIBRARY}
  ${GLEW_LIBRARY}
  ${AATOOLKIT_LIBRARY}
)

