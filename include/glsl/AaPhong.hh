#ifndef AA_GLSL_PHONG__H
#define AA_GLSL_PHONG__H

namespace Aa
{
  namespace GLSL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::Fragment::Phong ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    namespace Fragment
    {
      class Phong
      {
        private:
          Location<float> m_ambient;
          Location<float> m_diffuse;
          Location<float> m_specular;
          Location<float> m_shininess;

        public:
          Phong (Program * p) :
            m_ambient   (),
            m_diffuse   (),
            m_specular  (),
            m_shininess ()
          {
            if (p != NULL)
            {
              m_ambient   = p->location ("aa_phong_ambient");
              m_diffuse   = p->location ("aa_phong_diffuse");
              m_specular  = p->location ("aa_phong_specular");
              m_shininess = p->location ("aa_phong_shininess");
            }
          }

          void ambient   (float f) {m_ambient   (f);}
          void diffuse   (float f) {m_diffuse   (f);}
          void specular  (float f) {m_specular  (f);}
          void shininess (float f) {m_shininess (f);}
      };
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::Phong /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Phong :
      public TCoreProgram<Fragment::Phong>
    {
      public:
        Phong (const std::string & vertex   = "/Aa/Core.vertex",
               const std::string & fragment = "/Aa/Core.fragment") :
          TCoreProgram<Aa::GLSL::Fragment::Phong> (vertex, fragment)
        {
        }

        void ambient   (float f) {(*this)->ambient   (f);}
        void diffuse   (float f) {(*this)->diffuse   (f);}
        void specular  (float f) {(*this)->specular  (f);}
        void shininess (float f) {(*this)->shininess (f);}
    };

  }
}

#endif // AA_GLSL_PHONG__H

