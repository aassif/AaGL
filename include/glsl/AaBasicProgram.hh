#ifndef AA_GL_BASIC_PROGRAM__H
#define AA_GL_BASIC_PROGRAM__H

namespace Aa
{
  namespace GL
  {
    template <class V, class F>
    class TBasicProgram : public Program
    {
      public:
        typedef V Vertex;
        typedef F Fragment;

      protected:
        Vertex   * m_vertex;
        Fragment * m_fragment;

      public:
        TBasicProgram (const std::string & vertex,
                       const std::string & fragment) :
          Program    (),
          m_vertex   (NULL),
          m_fragment (NULL)
        {
          this->attach (GL_VERTEX_SHADER,   vertex);
          this->attach (GL_FRAGMENT_SHADER, fragment);
          this->link ();

          m_vertex   = new Vertex   (this);
          m_fragment = new Fragment (this);
        }

        ~TBasicProgram ()
        {
          delete m_vertex;
          delete m_fragment;
        }

        Vertex   * vertex   () {return m_vertex;}
        Fragment * fragment () {return m_fragment;}
    };
  }
}

#endif // AA_GL_BASIC_PROGRAM__H

