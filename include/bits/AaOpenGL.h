#ifndef __AA_OPENGL__
#define __AA_OPENGL__

#include <GL/glew.h>
#include <AaMath>

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

namespace Aa
{
  namespace GL
  {

    AA_GL_API inline void Vertex (const Math::vR3 & v) {glVertex3d (v.x, v.y, v.z);}
    AA_GL_API inline void Normal (const Math::vR3 & n) {glNormal3d (n.x, n.y, n.z);}

    AA_GL_API inline void Translate (const Math::pR3 & t) {glTranslated (t.x, t.y, t.z);}
    AA_GL_API inline void Translate (const Math::vR3 & t) {glTranslated (t.x, t.y, t.z);}
    AA_GL_API inline void Scale     (const Math::vR3 & s) {glScaled     (s.x, s.y, s.z);}

  } // namespace GL
} // namespace Aa

#endif // __AA_OPENGL__

