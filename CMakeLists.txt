# Required CMake version.

cmake_minimum_required(VERSION 2.8)

# Project name.

project(AaOpenGL C CXX)

include("$ENV{DEVEL_PATH}/cmake/cmake-paths.txt")
include("$ENV{DEVEL_PATH}/cmake/cmake-env.txt"  )

if(MSVC)
    set(CMAKE_CXX_FLAGS "/EHa")
endif(MSVC)

# Dependencies.

find_package(OpenGL     REQUIRED)
find_package(GLEW       REQUIRED)
find_package(AaToolkit  REQUIRED)

# Include directoires.

include_directories(
    include
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${AATOOLKIT_INCLUDE_DIR}
)

# Files.

file(GLOB HEADER_FILES include/Aa* include/bits/Aa*)
file(GLOB SOURCE_FILES src/Aa*)

# Shared target generation.

add_library(AaOpenGL
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${GLEW_FILES}
)

set_target_properties(AaOpenGL
    PROPERTIES
    COMPILE_DEFINITIONS AA_GL_BUILD_SHARED
)

target_link_libraries(AaOpenGL
    ${OPENGL_LIBRARY}
)

# Static target generation.

add_library(AaOpenGLs
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${GLEW_FILES}
)

target_link_libraries(AaOpenGLs
    ${OPENGL_LIBRARY}
)
