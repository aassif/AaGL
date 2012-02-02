#ifndef __AA_GL__
#define __AA_GL__

//#include <GL/glew.h>
//#include <AaMath>

namespace Aa
{
  namespace GL
  {

    // OpenGL ligthing.
    class AA_GL_API Lighting
    {
      public:
        // glColor.
        static void Color (const float [4]);
        // glMaterial.
        static void Material (const float [4]);
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL__

