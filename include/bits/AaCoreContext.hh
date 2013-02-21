#ifndef AA_GL_CORE_CONTEXT__H
#define AA_GL_CORE_CONTEXT__H

namespace Aa
{
  namespace GL
  {

    class CoreContext
    {
      public:
        inline static
        mat3 NormalMatrix (const mat4 & modelview)
        {
          return mat3 (modelview).inv ().transpose ();
        }

      private:
        Program * m_program;
        mat4      m_modelview;
        mat4      m_projection;
        vec2      m_depth_range;
        bool      m_moving;

      public:
        inline
        CoreContext (Program    * program,
                     const mat4 & modelview,
                     const mat4 & projection,
                     const vec2 & depth_range,
                     bool         moving = false) :
          m_program     (program),
          m_modelview   (modelview),
          m_projection  (projection),
          m_depth_range (depth_range),
          m_moving      (moving)
        {
        }

        inline Program    * program     () const {return m_program;}
        inline const mat4 & modelview   () const {return m_modelview;}
        inline const mat4 & projection  () const {return m_projection;}
        inline const vec2 & depth_range () const {return m_depth_range;}
        inline bool         is_moving   () const {return m_moving;}

        inline
        void use () const
        {
          m_program->use ();
          m_program->set<GLfloat, 4, 4> ("aa_gl_modelview",          m_modelview);
          m_program->set<GLfloat, 4, 4> ("aa_gl_modelview_inverse",  m_modelview.inv ());
          m_program->set<GLfloat, 4, 4> ("aa_gl_projection",         m_projection);
          m_program->set<GLfloat, 4, 4> ("aa_gl_projection_inverse", m_projection.inv ());
          m_program->set<GLfloat, 3, 3> ("aa_gl_normal_matrix",      NormalMatrix (m_modelview));
          m_program->set<GLfloat, 2>    ("aa_gl_depth_range",        m_depth_range);
        }
    };

  } // namespace GL
} //namespace Aa

#endif // AA_GL_CORE_CONTEXT__H

