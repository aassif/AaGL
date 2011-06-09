#ifndef __AA_GL__
#define __AA_GL__

#include <GL/glew.h>
#include <AaMath>

namespace Aa
{
  namespace GL
  {

    // OpenGL commands.
    inline static void Vertex (const Math::vR3 & v) {glVertex3d (v.x, v.y, v.z);}
    inline static void Normal (const Math::vR3 & n) {glNormal3d (n.x, n.y, n.z);}

    inline static void Translate (const Math::pR3 & t) {glTranslated (t.x, t.y, t.z);}
    inline static void Translate (const Math::vR3 & t) {glTranslated (t.x, t.y, t.z);}
    inline static void Scale     (const Math::vR3 & s) {glScaled     (s.x, s.y, s.z);}

    // OpenGL ligthing.
    class Lighting
    {
      public:
        // glColor.
        static void Color (const float [4]);
        // glMaterial.
        static void Material (const float [4]);
    };

  }
}

#endif // __AA_GL__

