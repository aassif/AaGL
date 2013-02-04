# Required CMake version.
cmake_minimum_required(VERSION 2.8)

# Project name.
project(AaOpenGL C CXX)

include("$ENV{DEVEL_PATH}/cmake/cmake-paths.txt")

set(CMAKE_CXX_FLAGS "/EHa")

if(CMAKE_GENERATOR MATCHES ".* Win64")
    add_definitions(-DAA_GL_X64_LIBRARY)

    set(ARCHITECTURE_DIR x64)

    set(CMAKE_RELEASE_POSTFIX -x64)
    
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX -x64d)
    endif(WIN32)

    if(APPLE)
        set(CMAKE_DEBUG_POSTFIX -x64_debug)
    endif(APPLE)
else(CMAKE_GENERATOR MATCHES ".* Win64")
    set(ARCHITECTURE_DIR .)

    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX d)
    endif(WIN32)

    if(APPLE)
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif(APPLE)
endif(CMAKE_GENERATOR MATCHES ".* Win64")


# OpenGL dependency.
find_package(OpenGL REQUIRED) # GLEW

# glew dependency
find_package(GLEW REQUIRED)

# AaToolkit dependency
find_package(AaToolkit REQUIRED)

# Include directoires.
include_directories(
    include
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${AATOOLKIT_INCLUDE_DIR}
)

# Include files.
file(GLOB HEADER_FILES
    include/Aa*
    include/bits/Aa*
)

# Source files.
file(GLOB SOURCE_FILES
    src/Aa*
)

# Target generation.
add_library(AaOpenGL
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Output paths.
set_target_properties(AaOpenGL
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug/${ARCHITECTURE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/lib/debug/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release/${ARCHITECTURE_DIR}"
    COMPILE_DEFINITIONS AA_GL_BUILD_SHARED
)

# Target dependencies.
target_link_libraries(AaOpenGL
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
)

# Target generation.
add_library(AaOpenGL-static
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Output paths.
set_target_properties(AaOpenGL-static
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/lib/debug/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release/${ARCHITECTURE_DIR}"
)

# Target dependencies.
target_link_libraries(AaOpenGL-static
    ${OPENGL_LIBRARY}
    ${GLEW_LIBRARY}
)
