#ifndef AA_GLSL_CORE__H
#define AA_GLSL_CORE__H

namespace Aa
{
  namespace GLSL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::Vertex::Core //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    namespace Vertex
    {
      class Core
      {
        private:
          Location<mat4> m_modelview;
          Location<mat4> m_projection;
          Location<mat3> m_normal_matrix;
          Location<vec2> m_depth_range;

        public:
          Core (Program * p) :
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

          void context (const GL::CoreContext & c)
          {
            const mat4 & m = c.modelview ();

            m_modelview     (m);
            m_projection    (c.projection ());
            m_normal_matrix (GL::CoreContext::NormalMatrix (m));
            m_depth_range   (c.depth_range ());
          }
      };
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::TCoreProgram<F> /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    template <class F>
    class TCoreProgram :
      public TBasicProgram<Vertex::Core, F>
    {
      public:
        typedef TBasicProgram<Vertex::Core, F> Parent;

      public:
        TCoreProgram (const std::string & vertex,
                      const std::string & fragment) :
          Parent (vertex, fragment)
        {
        }

        void modelview     (const mat4 & m) {Parent::m_vertex->modelview     (m);}
        void projection    (const mat4 & m) {Parent::m_vertex->projection    (m);}
        void normal_matrix (const mat3 & m) {Parent::m_vertex->normal_matrix (m);}
        void depth_range   (const vec2 & v) {Parent::m_vertex->depth_range   (v);}

        void context (const GL::CoreContext & c)
        {
          Parent::m_vertex->context (c);
        }

        F * operator-> () {return Parent::m_fragment;}
    };

  }
}

#endif // AA_GLSL_CORE__H

