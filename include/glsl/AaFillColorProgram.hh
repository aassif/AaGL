#ifndef AA_GL_FILL_COLOR_PROGRAM__H
#define AA_GL_FILL_COLOR_PROGRAM__H

namespace Aa
{
  namespace GL
  {
    class FillColorProgram :
      public TCoreProgram<FillColorFragment>
    {
      public:
        typedef TCoreProgram<FillColorFragment> Parent;

      public:
        FillColorProgram (const std::string & vertex   = "/Aa/Core.vertex",
                          const std::string & fragment = "/Aa/FillColor.fragment") :
          TCoreProgram<FillColorFragment> (vertex, fragment)
        {
        }

        void fill_color (const vec4 & v) {(*this)->fill_color (v);}
    };
  }
}

#endif // AA_GL_FILL_COLOR_PROGRAM__H

