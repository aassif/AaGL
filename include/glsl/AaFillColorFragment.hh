#ifndef AA_GL_FILL_COLOR_FRAGMENT__H
#define AA_GL_FILL_COLOR_FRAGMENT__H

namespace Aa
{
  namespace GL
  {
    class FillColorFragment
    {
      private:
        Location<vec4> m_fill_color;

      public:
        FillColorFragment (Program * p) :
          m_fill_color ()
        {
          if (p != NULL)
            m_fill_color = p->location ("aa_gl_fill_color");
        }

        void fill_color (const vec4 & v) {m_fill_color (v);}
    };
  }
}

#endif // AA_GL_FILL_COLOR_FRAGMENT__H

