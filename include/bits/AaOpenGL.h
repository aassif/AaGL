#ifndef __AA_GL__
#define __AA_GL__

#ifdef _WIN32
  #ifdef AaToolkit_EXPORTS // Merci CMake :)
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

#endif // __AA_GL__

