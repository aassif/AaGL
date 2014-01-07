#ifndef AA_GL_PHONG_PROGRAM__H
#define AA_GL_PHONG_PROGRAM__H

namespace Aa
{
  namespace GL
  {
    class PhongProgram :
      public TCoreProgram<PhongFragment>
    {
      public:
        typedef TCoreProgram<PhongFragment> Parent;

      public:
        PhongProgram (const std::string & vertex   = "/Aa/Core.vertex",
                      const std::string & fragment = "/Aa/Core.fragment") :
          Parent (vertex, fragment)
        {
        }

        void ambient   (float f) {(*this)->ambient   (f);}
        void diffuse   (float f) {(*this)->diffuse   (f);}
        void specular  (float f) {(*this)->specular  (f);}
        void shininess (float f) {(*this)->shininess (f);}
    };
  }
}

#endif // AA_GL_PHONG_PROGRAM__H

