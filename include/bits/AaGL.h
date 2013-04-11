#ifndef AA_GL__H
#define AA_GL__H

#include <GL/glew.h>
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

