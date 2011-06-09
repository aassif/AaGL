//#include <cmath>
//#include <sstream>
//#include <iomanip>
//#include <GL/glew.h>
//#include <GL/glut.h>
//#include <AaTimer>
#include "AaGL"

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

    void Lighting::Color (const float c [4])
    {
      glDisable (GL_LIGHTING);
      glColor4fv (c);
    }

    void Lighting::Material (const float c [4])
    {
      glEnable (GL_LIGHTING);
      static const float ZERO [] = {0.0, 0.0, 0.0, 1.0};
      glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT,   ZERO);
      glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE,   c);
      glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR,  c);
      glMaterialf  (GL_FRONT_AND_BACK, GL_SHININESS, 64.0);
    }

  }
}

