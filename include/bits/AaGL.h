#ifndef __AA_GL__
#define __AA_GL__

#include <GL/glew.h>
#include <AaMatrix>
#include <AaMath>
//#include <AaBox> // FIXME

#define AA_GL_INLINE inline

#if defined( _WIN32 ) && defined( AA_OPENGL_BUILD_SHARED )
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
// Vertex //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_VERTEX(T, m, F) \
    AA_GL_INLINE \
    void Vertex (const V<T, m> & v) {F (&(v[0]));}

    AA_GL_VERTEX (GLshort,  2, glVertex2sv)
    AA_GL_VERTEX (GLint,    2, glVertex2iv)
    AA_GL_VERTEX (GLfloat,  2, glVertex2fv)
    AA_GL_VERTEX (GLdouble, 2, glVertex2dv)

    AA_GL_VERTEX (GLshort,  3, glVertex3sv)
    AA_GL_VERTEX (GLint,    3, glVertex3iv)
    AA_GL_VERTEX (GLfloat,  3, glVertex3fv)
    AA_GL_VERTEX (GLdouble, 3, glVertex3dv)

    AA_GL_VERTEX (GLshort,  4, glVertex4sv)
    AA_GL_VERTEX (GLint,    4, glVertex4iv)
    AA_GL_VERTEX (GLfloat,  4, glVertex4fv)
    AA_GL_VERTEX (GLdouble, 4, glVertex4dv)

////////////////////////////////////////////////////////////////////////////////
// Normal //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_NORMAL(T, F) \
    AA_GL_INLINE \
    void Normal (const V<T, 3> & n) {F (&(n[0]));}

    AA_GL_NORMAL (GLbyte,   glNormal3bv)
    AA_GL_NORMAL (GLshort,  glNormal3sv)
    AA_GL_NORMAL (GLint,    glNormal3iv)
    AA_GL_NORMAL (GLfloat,  glNormal3fv)
    AA_GL_NORMAL (GLdouble, glNormal3dv)

////////////////////////////////////////////////////////////////////////////////
// TexCoord ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_TEXCOORD1(T, F) \
    AA_GL_INLINE \
    void TexCoord (T t) {F (t);}

    AA_GL_TEXCOORD1 (GLshort,  glTexCoord1s)
    AA_GL_TEXCOORD1 (GLint,    glTexCoord1i)
    AA_GL_TEXCOORD1 (GLfloat,  glTexCoord1f)
    AA_GL_TEXCOORD1 (GLdouble, glTexCoord1d)

#define AA_GL_TEXCOORD(T, m, F) \
    AA_GL_INLINE \
    void TexCoord (const V<T, m> & v) {F (&(v[0]));}

    AA_GL_TEXCOORD (GLshort,  2, glTexCoord2sv)
    AA_GL_TEXCOORD (GLint,    2, glTexCoord2iv)
    AA_GL_TEXCOORD (GLfloat,  2, glTexCoord2fv)
    AA_GL_TEXCOORD (GLdouble, 2, glTexCoord2dv)

    AA_GL_TEXCOORD (GLshort,  3, glTexCoord3sv)
    AA_GL_TEXCOORD (GLint,    3, glTexCoord3iv)
    AA_GL_TEXCOORD (GLfloat,  3, glTexCoord3fv)
    AA_GL_TEXCOORD (GLdouble, 3, glTexCoord3dv)

    AA_GL_TEXCOORD (GLshort,  4, glTexCoord4sv)
    AA_GL_TEXCOORD (GLint,    4, glTexCoord4iv)
    AA_GL_TEXCOORD (GLfloat,  4, glTexCoord4fv)
    AA_GL_TEXCOORD (GLdouble, 4, glTexCoord4dv)

////////////////////////////////////////////////////////////////////////////////
// Translate ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_TRANSLATE(T, F) \
    AA_GL_INLINE \
    void Translate (const V<T, 3> & v) {F (v[0], v[1], v[2]);}

    AA_GL_TRANSLATE (GLfloat,  glTranslatef)
    AA_GL_TRANSLATE (GLdouble, glTranslated)

////////////////////////////////////////////////////////////////////////////////
// Scale ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_SCALE(T, F) \
    AA_GL_INLINE \
    void Scale (const V<T, 3> & v) {F (v[0], v[1], v[2]);}

    AA_GL_SCALE (GLfloat,  glScalef)
    AA_GL_SCALE (GLdouble, glScaled)

////////////////////////////////////////////////////////////////////////////////
// Rotate //////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_ROTATE(T, F) \
    AA_GL_INLINE \
    void Rotate (const T & a, const V<T, 3> & v) {F (a, v[0], v[1], v[2]);}

    AA_GL_ROTATE (GLfloat,  glRotatef)
    AA_GL_ROTATE (GLdouble, glRotated)

////////////////////////////////////////////////////////////////////////////////
// Project /////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

    AA_GL_INLINE
    dvec3 Project (const dvec3 & object)
    {
      GLdouble modelview [16];
      glGetDoublev (GL_MODELVIEW_MATRIX, modelview);

      GLdouble projection [16];
      glGetDoublev (GL_PROJECTION_MATRIX, projection);

      GLint viewport [4];
      glGetIntegerv (GL_VIEWPORT, viewport);

      return Project (object, modelview, projection, viewport);
    }

    AA_GL_INLINE
    dvec3 Project (const dvec3 & object,
                   const dmat4 & modelview,
                   const dmat4 & projection,
                   const ibox2 & v)
    {
      const ivec2 & pos = v.pos ();
      const ivec2 & dim = v.dim ();
      GLint viewport [4] = {pos[0], pos[1], dim[0], dim[1]};
      return Project (object, &(modelview[0][0]), &(projection[0][0]), viewport);
    }

////////////////////////////////////////////////////////////////////////////////
// UnProject ///////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

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

    AA_GL_INLINE
    dvec3 UnProject (const dvec3 & window)
    {
      GLdouble modelview [16];
      glGetDoublev (GL_MODELVIEW_MATRIX, modelview);

      GLdouble projection [16];
      glGetDoublev (GL_PROJECTION_MATRIX, projection);

      GLint viewport [4];
      glGetIntegerv (GL_VIEWPORT, viewport);

      return UnProject (window, modelview, projection, viewport);
    }

    AA_GL_INLINE
    dvec3 UnProject (const dvec3 & window,
                     const dmat4 & modelview,
                     const dmat4 & projection,
                     const ibox2 & v)
    {
      const ivec2 & pos = v.pos ();
      const ivec2 & dim = v.dim ();
      GLint viewport [4] = {pos[0], pos[1], dim[0], dim[1]};
      return UnProject (window, &(modelview[0][0]), &(projection[0][0]), viewport);
    }

  } // namespace GL
} // namespace Aa

#endif // __AA_GL__

