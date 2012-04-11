#ifndef __AA_GL_PRIMITIVES__
#define __AA_GL_PRIMITIVES__

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
      private:
        static const double CONTOUR [][2];
      public:
        // Unit box.
        static void Box ();
        // A custom box.
        static void Box (const dbox3 &);
        // A disk at (0, 0, 0) winth radius r on the z plane.
        static void Disk (double r);
        // A sphere at (0, 0, 0) with radius r.
        static void Sphere (double r);
        // A sphere at c with radius r.
        static void Sphere (const dvec3 & c, double r);
        // A cone with height h and radii r1 and r2.
        static void Cone (double h, double r1, double r2, bool closed = false);
        // A cone between 2 points with radii r1 and r2.
        static void Cone (const dvec3 & p1, double r1, const dvec3 & p2, double r2, bool closed = false);
        // A cylinder with height h and radius r.
        static void Cylinder (double h, double r, bool closed = false);
        // A cylinder between 2 points with radius r.
        static void Cylinder (const dvec3 & p1, const dvec3 & p2, double r, bool closed = false);
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL_PRIMITIVES__

