#ifndef __AA_OPENGL__
#define __AA_OPENGL__

#ifdef _WIN32
  #ifdef AaOpenGL_EXPORTS // Merci CMake :)
    #define AA_GL_API    __declspec(dllexport)
    #define AA_GL_EXTERN
  #else
    #define AA_GL_API    __declspec(dllimport)
    #define AA_GL_EXTERN extern
  #endif
#else
  #define AA_GL_API
  #define AA_GL_EXTERN
#endif

#endif // __AA_OPENGL__

