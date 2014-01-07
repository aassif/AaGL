#ifndef AA_GL_PHONG_FRAGMENT__H
#define AA_GL_PHONG_FRAGMENT__H

namespace Aa
{
  namespace GL
  {
    class PhongFragment
    {
      private:
        Location<float> m_ambient;
        Location<float> m_diffuse;
        Location<float> m_specular;
        Location<float> m_shininess;

      public:
        PhongFragment (Program * p) :
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
}

#endif // AA_GL_PHONG_FRAGMENT__H

