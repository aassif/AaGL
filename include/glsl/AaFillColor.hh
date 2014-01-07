#ifndef AA_GLSL_FILL_COLOR__H
#define AA_GLSL_FILL_COLOR__H

namespace Aa
{
  namespace GLSL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::Fragment::FillColor ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    namespace Fragment
    {
      class FillColor
      {
        private:
          Location<vec4> m_fill_color;

        public:
          FillColor (Program * p) :
            m_fill_color ()
          {
            if (p != NULL)
              m_fill_color = p->location ("aa_gl_fill_color");
          }

          void fill_color (const vec4 & v) {m_fill_color (v);}
      };
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::FillColor /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class FillColor :
      public TCoreProgram<Fragment::FillColor>
    {
      public:
        FillColor (const std::string & vertex   = "/Aa/Core.vertex",
                   const std::string & fragment = "/Aa/FillColor.fragment") :
          TCoreProgram<Aa::GLSL::Fragment::FillColor> (vertex, fragment)
        {
        }

        void fill_color (const vec4 & v) {(*this)->fill_color (v);}
    };

  }
}

#endif // AA_GLSL_FILL_COLOR__H

