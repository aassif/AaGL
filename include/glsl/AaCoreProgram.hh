#ifndef AA_GL_CORE_PROGRAM__H
#define AA_GL_CORE_PROGRAM__H

namespace Aa
{
  namespace GL
  {
    template <class F>
    class TCoreProgram :
      public TBasicProgram<CoreVertex, F>
    {
      public:
        typedef TBasicProgram<CoreVertex, F> Parent;

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

        F * operator-> () {return Parent::m_fragment;}
    };
  }
}

#endif // AA_GL_BASIC_PROGRAM__H

