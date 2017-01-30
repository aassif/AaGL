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

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Viewport ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    AA_GL_INLINE
    ibox2 Viewport ()
    {
      ivec4 v;
      glGetIntegerv (GL_VIEWPORT, &(v[0]));
      return ibox2 (ivec2 (v[0], v[1]), ivec2 (v[2], v[3]));
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Project /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __APPLE__
    AA_GL_INLINE
    dvec3 Project (const dvec3    & object,
                   const GLdouble * modelview,
                   const GLdouble * projection,
                   const GLint    * viewport)
    {
      dvec3 p;
      gluProject (object[0], object[1], object[2],
                  modelview, projection, viewport,
                  &(p[0]), &(p[1]), &(p[2]));
      return p;
    }
#endif

    AA_GL_INLINE
    dvec3 Project (const dvec3 & object,
                   const dmat4 & modelview,
                   const dmat4 & projection,
                   const ibox2 & viewport)
    {
#ifdef __APPLE__
      dvec4 o = projection * modelview * dvec4 (object, 1);
      ivec3 vp = ivec3 (viewport.pos  (), 0);
      ivec3 vd = ivec3 (viewport.dims (), 1);
      return vp + 0.5 * (1 + dvec3 (o / o [3]) * vd);
#else
      const ivec2 & p = viewport.pos  ();
      const ivec2 & d = viewport.dims ();
      GLint v [4] = {p[0], p[1], d[0], d[1]};
      return Project (object, &(modelview[0][0]), &(projection[0][0]), v);
#endif
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::UnProject ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __APPLE__
    AA_GL_INLINE
    dvec3 UnProject (const dvec3    & window,
                     const GLdouble * modelview,
                     const GLdouble * projection,
                     const GLint    * viewport)
    {
      dvec3 p;
      gluUnProject (window[0], window[1], window[2],
                    modelview, projection, viewport,
                    &(p[0]), &(p[1]), &(p[2]));
      return p;
    }
#endif

    AA_GL_INLINE
    dvec3 UnProject (const dvec3 & window,
                     const dmat4 & modelview,
                     const dmat4 & projection,
                     const ibox2 & viewport)
    {
#ifdef __APPLE__
      ivec3 vp = ivec3 (viewport.pos  (), 0);
      ivec3 vd = ivec3 (viewport.dims (), 1);
      dvec4 p = dvec4 (2.0 * (window - vp) / vd - 1, 1);
      std::cout << "Aa::GL::UnProject " << p << std::endl;
      dvec4 u = mat4 (projection * modelview).inv () * p;
      return u / u [3];
#else
      const ivec2 & p = viewport.pos  ();
      const ivec2 & d = viewport.dims ();
      GLint v [4] = {p[0], p[1], d[0], d[1]};
      return UnProject (window, &(modelview[0][0]), &(projection[0][0]), v);
#endif
    }

  } // namespace GL
} // namespace Aa

#endif // AA_GL__H

