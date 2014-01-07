#ifndef AA_GL_CORE_VERTEX__H
#define AA_GL_CORE_VERTEX__H

namespace Aa
{
  namespace GL
  {
    class CoreVertex
    {
      private:
        Location<mat4> m_modelview;
        Location<mat4> m_projection;
        Location<mat3> m_normal_matrix;
        Location<vec2> m_depth_range;

      public:
        CoreVertex (Program * p) :
          m_modelview     (),
          m_projection    (),
          m_normal_matrix (),
          m_depth_range   ()
        {
          if (p != NULL)
          {
            m_modelview     = p->location ("aa_gl_modelview");
            m_projection    = p->location ("aa_gl_projection");
            m_normal_matrix = p->location ("aa_gl_normal_matrix");
            m_depth_range   = p->location ("aa_gl_depth_range");
          }
        }

        void modelview     (const mat4 & m) {m_modelview     (m);}
        void projection    (const mat4 & m) {m_projection    (m);}
        void normal_matrix (const mat3 & m) {m_normal_matrix (m);}
        void depth_range   (const vec2 & v) {m_depth_range   (v);}
    };
  }
}

#endif // AA_GL_CORE_VERTEX__H

