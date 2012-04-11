//#include <cmath>
//#include <sstream>
//#include <iomanip>
//#include <GL/glew.h>
//#include <GL/glut.h>
//#include <AaTimer>
#include "AaLight"

//#define AA_GL_UPSIDE_DOWN

using namespace std;
//using namespace Aa;

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Lighting ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    void Lighting::Color (const vec4 & c)
    {
      glDisable (GL_LIGHTING);
      glColor4fv (&(c[0]));
    }

    void Lighting::Material (const vec4 & c)
    {
      glEnable (GL_LIGHTING);
      static const float ZERO [] = {0.0, 0.0, 0.0, 1.0};
      glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ZERO);
      glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   &(c[0]));
      glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  &(c[0]));
      glMaterialf  (GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
    }

  }
}

