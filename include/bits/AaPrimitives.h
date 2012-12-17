#ifndef AA_GL_PRIMITIVES__H
#define AA_GL_PRIMITIVES__H

#include <vector>
//#include "AaGL"

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Primitives //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_GL_API Primitives
    {
      public:
        // Unit box.
        static void Box ();
        // A custom box.
        static void Box (const box3 &);
        // A disk at (0, 0, 0) with radius r on the z plane.
        static void Disk (float r);
        // A sphere at (0, 0, 0) with radius r.
        static void Sphere (float r);
        // A sphere at c with radius r.
        static void Sphere (const vec3 & c, float r);
        // A cone with height h and radii r1 and r2.
        static void Cone (float h, float r1, float r2, bool closed = false);
        // A cone between 2 points with radii r1 and r2.
        static void Cone (const vec3 & p1, float r1, const vec3 & p2, float r2, bool closed = false);
        // A cylinder with height h and radius r.
        static void Cylinder (float h, float r, bool closed = false);
        // A cylinder between 2 points with radius r.
        static void Cylinder (const vec3 & p1, const vec3 & p2, float r, bool closed = false);
    };

  } // namespace GL
} // namespace Aa

#endif // AA_GL_PRIMITIVES__H

