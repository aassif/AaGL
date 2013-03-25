# Library.
if(MSVC)
    set(AAOPENGL_GENERATOR_DIR win32-msvc)
endif(MSVC)

if(CMAKE_GENERATOR MATCHES "MinGW*")
    set(AAOPENGL_GENERATOR_DIR win32-g++)
endif(CMAKE_GENERATOR MATCHES "MinGW*")

if(CMAKE_GENERATOR MATCHES ".* Win64")
    find_library(AAOPENGL_LIBRARY_RELEASE
        NAMES ${AAOPENGL_LIBRARY_BASE_NAME}-x64
        PATHS "${AAOPENGL_PATH}/lib/${AAOPENGL_GENERATOR_DIR}/x64"
    )

    find_library(AAOPENGL_LIBRARY_DEBUG
        NAMES ${AAOPENGL_LIBRARY_BASE_NAME}d-x64
        PATHS "${AAOPENGL_PATH}/lib/${AAOPENGL_GENERATOR_DIR}/x64/debug"
    )
else(CMAKE_GENERATOR MATCHES ".* Win64")
    find_library(AAOPENGL_LIBRARY_RELEASE
        NAMES ${AAOPENGL_LIBRARY_BASE_NAME}
        PATHS "${AAOPENGL_PATH}/lib/${AAOPENGL_GENERATOR_DIR}"
    )

    find_library(AAOPENGL_LIBRARY_DEBUG
        NAMES ${AAOPENGL_LIBRARY_BASE_NAME}d
        PATHS "${AAOPENGL_PATH}/lib/${AAOPENGL_GENERATOR_DIR}/debug"
    )
endif(CMAKE_GENERATOR MATCHES ".* Win64")

if(NOT AAOPENGL_LIBRARY_DEBUG)
    set(AAOPENGL_LIBRARY_DEBUG "AAOPENGL_LIBRARY_DEBUG-NOPATH")
endif(NOT AAOPENGL_LIBRARY_DEBUG)

set(AAOPENGL_LIBRARY
    debug     "${AAOPENGL_LIBRARY_DEBUG}"
    optimized "${AAOPENGL_LIBRARY_RELEASE}"
)

# Include.
find_path(AAOPENGL_INCLUDE_DIR
    NAMES
    AaCoreContext
    AaFBO
    AaFixedFunction
    AaGL
    AaGLFF
    AaGLSL
    AaGlutMenu
    AaGlutText
    AaGlutWatermark
    AaGrabber
    AaLight
    AaPrimitives
    AaReferenceFrame
    AaScene
    bits/AaCoreContext.hh
    bits/AaFBO.hh
    bits/AaFixedFunction.hh
    bits/AaGL.h
    bits/AaGLFF.hh
    bits/AaGLSL.h
    bits/AaGlutMenu.hh
    bits/AaGlutText.hh
    bits/AaGlutWatermark.hh
    bits/AaGrabber.h
    bits/AaLight.h
    bits/AaPrimitives.h
    bits/AaReferenceFrame.hh
    bits/AaScene.h
    PATHS
    "${AAOPENGL_PATH}/include"
)

if(AAOPENGL_INCLUDE_DIR AND AAOPENGL_LIBRARY)
    set(AAOPENGL_FOUND TRUE)
endif(AAOPENGL_INCLUDE_DIR AND AAOPENGL_LIBRARY)

if(AAOPENGL_FOUND)
    if(NOT AAOPENGL_FIND_QUIETLY)
        message(STATUS "Found AAOPENGL : ${AAOPENGL_LIBRARY}")
    endif(NOT AAOPENGL_FIND_QUIETLY)
else(AAOPENGL_FOUND)
    if(AAOPENGL_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find AAOPENGL")
    endif(AAOPENGL_FIND_REQUIRED)
endif(AAOPENGL_FOUND)
