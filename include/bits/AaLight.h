#ifndef __AA_GL_LIGHT__
#define __AA_GL_LIGHT__

//#include <GL/glew.h>
//#include <AaMath>
#include <AaColor>

namespace Aa
{
  namespace GL
  {

    // OpenGL ligthing.
    class AA_GL_API Lighting
    {
      public:
        // glColor.
        static void Color (const vec4 &);
        // glMaterial.
        static void Material (const vec4 &);
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL_LIGHT__

