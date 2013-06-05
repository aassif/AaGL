#ifndef AA_GL_FIXED_FUNCTION__H
#define AA_GL_FIXED_FUNCTION__H

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Vertex //////////////////////////////////////////////////////////////
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
// Aa::GL::Color ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_COLOR(T, m, F) \
    AA_GL_INLINE \
    void Color (const V<T, m> & v) {F (&(v[0]));}

    AA_GL_COLOR (GLbyte,   3, glColor3bv)
    AA_GL_COLOR (GLubyte,  3, glColor3ubv)
    AA_GL_COLOR (GLshort,  3, glColor3sv)
    AA_GL_COLOR (GLushort, 3, glColor3usv)
    AA_GL_COLOR (GLint,    3, glColor3iv)
    AA_GL_COLOR (GLuint,   3, glColor3uiv)
    AA_GL_COLOR (GLfloat,  3, glColor3fv)
    AA_GL_COLOR (GLdouble, 3, glColor3dv)

    AA_GL_COLOR (GLbyte,   4, glColor4bv)
    AA_GL_COLOR (GLubyte,  4, glColor4ubv)
    AA_GL_COLOR (GLshort,  4, glColor4sv)
    AA_GL_COLOR (GLushort, 4, glColor4usv)
    AA_GL_COLOR (GLint,    4, glColor4iv)
    AA_GL_COLOR (GLuint,   4, glColor4uiv)
    AA_GL_COLOR (GLfloat,  4, glColor4fv)
    AA_GL_COLOR (GLdouble, 4, glColor4dv)

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Normal //////////////////////////////////////////////////////////////
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
// Aa::GL::TexCoord ////////////////////////////////////////////////////////////
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
// Aa::GL::Translate ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_TRANSLATE(T, F) \
    AA_GL_INLINE \
    void Translate (const V<T, 3> & v) {F (v[0], v[1], v[2]);}

    AA_GL_TRANSLATE (GLfloat,  glTranslatef)
    AA_GL_TRANSLATE (GLdouble, glTranslated)

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Scale ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_SCALE(T, F) \
    AA_GL_INLINE \
    void Scale (const V<T, 3> & v) {F (v[0], v[1], v[2]);}

    AA_GL_SCALE (GLfloat,  glScalef)
    AA_GL_SCALE (GLdouble, glScaled)

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Rotate //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_ROTATE(T, F) \
    AA_GL_INLINE \
    void Rotate (const T & a, const V<T, 3> & v) {F (a, v[0], v[1], v[2]);}

    AA_GL_ROTATE (GLfloat,  glRotatef)
    AA_GL_ROTATE (GLdouble, glRotated)

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::MultMatrix //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define AA_GL_MULT_MATRIX(T, F) \
    AA_GL_INLINE \
    void MultMatrix (const M<T, 4, 4> & m) {F (&(m[0][0]));}

    AA_GL_MULT_MATRIX (GLfloat,  glMultMatrixf)
    AA_GL_MULT_MATRIX (GLdouble, glMultMatrixd)

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::ModelView ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    AA_GL_INLINE
    mat4 ModelView ()
    {
      mat4 m;
      glGetFloatv (GL_MODELVIEW_MATRIX, &(m[0][0]));
      return m;
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Projection //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    AA_GL_INLINE
    mat4 Projection ()
    {
      mat4 m;
      glGetFloatv (GL_PROJECTION_MATRIX, &(m[0][0]));
      return m;
    }

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
    dvec3 Project (const dvec3 & object,
                   const dmat4 & modelview,
                   const dmat4 & projection,
                   const ibox2 & viewport)
    {
      const ivec2 & p = viewport.pos ();
      const ivec2 & d = viewport.dim ();
      GLint v [4] = {p[0], p[1], d[0], d[1]};
      return Project (object, &(modelview[0][0]), &(projection[0][0]), v);
    }

    AA_GL_INLINE
    dvec3 Project (const dvec3 & object)
    {
      return Project (object, ModelView (), Projection (), Viewport ());
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::UnProject ///////////////////////////////////////////////////////////
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

    AA_GL_INLINE
    dvec3 UnProject (const dvec3 & window)
    {
      return UnProject (window, ModelView (), Projection (), Viewport ());
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::FixedFunction ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    AA_GL_INLINE
    CoreContext FixedFunction (const vec2 & depth_range,
                               bool         motion = false)
    {
      return CoreContext (ModelView (),
                          Projection (),
                          depth_range,
                          motion);
    }

  } // namespace GL
} //namespace Aa

#endif // AA_GL_FIXED_FUNCTION__H

