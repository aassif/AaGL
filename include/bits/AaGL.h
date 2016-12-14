#ifndef AA_GL__H
#define AA_GL__H

#ifdef __APPLE__
  #include <OpenGL/gl3.h>
#else
  #include <GL/glew.h>
#endif

#include <AaMatrix>
#include <AaBox>

#define AA_GL_INLINE inline

#if defined (_WIN32) && defined (AA_GL_BUILD_SHARED)
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

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Check ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    inline
    void Check (const std::string & prefix)
    {
      GLenum e = glGetError ();
      switch (e)
      {
        case GL_NO_ERROR:                      break;
        case GL_INVALID_ENUM:                  throw std::runtime_error (prefix + ": " + "GL_INVALID_ENUM");
        case GL_INVALID_VALUE:                 throw std::runtime_error (prefix + ": " + "GL_INVALID_VALUE");
        case GL_INVALID_OPERATION:             throw std::runtime_error (prefix + ": " + "GL_INVALID_OPERATION");
        case GL_INVALID_FRAMEBUFFER_OPERATION: throw std::runtime_error (prefix + ": " + "GL_INVALID_FRAMEBUFFER_OPERATION");
        case GL_OUT_OF_MEMORY:                 throw std::runtime_error (prefix + ": " + "GL_GL_OUT_OF_MEMORY");
#ifndef __APPLE__
        case GL_STACK_UNDERFLOW:               throw std::runtime_error (prefix + ": " + "GL_STACK_UNDERFLOW");
        case GL_STACK_OVERFLOW:                throw std::runtime_error (prefix + ": " + "GL_STACK_OVERFLOW");
#endif
        default:                               throw std::runtime_error (prefix + ": " + "???");
      }
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::MissingExtension ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class MissingExtension : public std::runtime_error
    {
      public:
        MissingExtension (const std::string & ext) :
          std::runtime_error (ext)
        {
        }
    };

  } // namespace GL
} // namespace Aa

#endif // AA_GL__H

