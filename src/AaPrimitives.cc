#include <cmath>
//#include <sstream>
//#include <iomanip>
//#include <AaTimer>
#include "AaPrimitives"

using namespace std;
//using namespace Aa;

namespace Aa
{
  namespace GL
  {

//////////////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Primitives ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

    void Primitives::Box ()
    {
      // Blending.
      GLboolean backup_glBlend = glIsEnabled (GL_BLEND);
      glEnable (GL_BLEND);
      // Smooth lines.
      //GLboolean backup_glLineSmooth = glIsEnabled (GL_LINE_SMOOTH);
      //glEnable (GL_LINE_SMOOTH);
      //glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
      // Depth mask.
      //GLboolean backup_glDepthMask;
      //glGetBooleanv (GL_DEPTH_WRITEMASK, &backup_glDepthMask);
      //glDepthMask (GL_FALSE);
      // Polyong mode.
      GLint backup_glPolygonMode [2];
      glGetIntegerv (GL_POLYGON_MODE, backup_glPolygonMode);
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      // Bounding box vertices.
      static const GLdouble BOX_VERTICES [] = {
        0.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        0.0, 1.0, 1.0,
        1.0, 1.0, 1.0
      };

      // Bounding box faces.
      static const GLubyte BOX_FACES [] = {
        0, 1, 5, 4, // Bottom
        0, 2, 3, 1, // Back
        0, 4, 6, 2, // Left
        1, 3, 7, 5, // Right
        2, 6, 7, 3, // Top
        4, 5, 7, 6  // Front
      };

      glEnableClientState (GL_VERTEX_ARRAY);
      glVertexPointer (3, GL_DOUBLE, 0, BOX_VERTICES);
      glDrawElements (GL_QUADS, 24, GL_UNSIGNED_BYTE, BOX_FACES);
      glDisableClientState (GL_VERTEX_ARRAY);

      glPolygonMode (GL_FRONT, backup_glPolygonMode [0]);
      glPolygonMode (GL_BACK,  backup_glPolygonMode [1]);
      //if (backup_glDepthMask) glDepthMask (GL_TRUE);
      //if (! backup_glLineSmooth) glDisable (GL_LINE_SMOOTH);
      if (! backup_glBlend)      glDisable (GL_BLEND);
    }

    void Primitives::Box (const Math::Box & b)
    {
      glPushMatrix ();
      Translate (b.pos ());
      Scale     (b.dim ());
      Primitives::Box ();
      glPopMatrix ();
    }

    const double Primitives::CONTOUR [][2] = {
      {+1.000,  0.000},
      {+0.966, +0.259},
      {+0.866, +0.500},
      {+0.707, +0.707},
      {+0.500, +0.866},
      {+0.259, +0.966},
      { 0.000, +1.000},
      {-0.259, +0.966},
      {-0.500, +0.866},
      {-0.707, +0.707},
      {-0.866, +0.500},
      {-0.966, +0.259},
      {-1.000,  0.000},
      {-0.966, -0.259},
      {-0.866, -0.500},
      {-0.707, -0.707},
      {-0.500, -0.866},
      {-0.259, -0.966},
      { 0.000, -1.000},
      {+0.259, -0.966},
      {+0.500, -0.866},
      {+0.707, -0.707},
      {+0.866, -0.500},
      {+0.966, -0.259},
      {+1.000,  0.000}
    };

    void Primitives::Disk (double r)
    {
      int nSlices = 24;

      glBegin (GL_TRIANGLE_FAN);
      glNormal3d (0, 0, 1);
      glVertex2d (0, 0);
      for (int i = 0; i <= nSlices; ++i)
        glVertex2d (CONTOUR[i][0] * r, CONTOUR[i][1] * r);
      glEnd ();
    }

    void Primitives::Sphere (double r)
    {
      int nSlices = 24;
      int nStacks = 12;

      for (int i = 0; i < nStacks; ++i)
      {
        double cos_a1 = CONTOUR[i][0];
        double sin_a1 = CONTOUR[i][1];

        double cos_a2 = CONTOUR[i+1][0];
        double sin_a2 = CONTOUR[i+1][1];

        glBegin (GL_QUAD_STRIP);
        for (int i = 0; i <= nSlices; ++i)
        {
          double cos_b = CONTOUR[i][0];
          double sin_b = CONTOUR[i][1];
          // Vertex i on 1st contour.
          glNormal3d (    cos_b * sin_a1,     sin_b * sin_a1,     cos_a1);
          glVertex3d (r * cos_b * sin_a1, r * sin_b * sin_a1, r * cos_a1);
          // Vertex i on 2nd contour.
          glNormal3d (    cos_b * sin_a2,     sin_b * sin_a2,     cos_a2);
          glVertex3d (r * cos_b * sin_a2, r * sin_b * sin_a2, r * cos_a2);
        }
        glEnd ();
      }

      //glutSolidSphere (r, nSlices, nStacks);
    }

    void Primitives::Sphere (const Math::pR3 & c, double r)
    {
      glPushMatrix ();
      Translate (c);
      Primitives::Sphere (r);
      glPopMatrix ();
    }

    void Primitives::Cone (double h, double r1, double r2, bool closed)
    {
      //static GLUquadricObj * cylinder = NULL;
      //if (cylinder == NULL) cylinder = gluNewQuadric ();

      int nSlices = 8;
      
      if (closed)
      {
        glPushMatrix ();
        glRotated (180.0, 1.0, 0.0, 0.0); // FIXME
        //gluDisk (cylinder, 0, r1, 16, 1);
        Primitives::Disk (r1);
        glPopMatrix ();
      }

      //gluCylinder (cylinder, r1, r2, h, 16, 1);

      glBegin (GL_QUAD_STRIP);
      double a = std::atan2 (h, r2 - r1);
      double cos_a = std::cos (a);
      double sin_a = std::sin (a);
      for (int i = 0; i <= nSlices; ++i)
      {
        double b = (2.0 * M_PI * i) / nSlices;
        double cos_b = std::cos (b); //CONTOUR[i][0];
        double sin_b = std::sin (b); //CONTOUR[i][1];
        glNormal3d (cos_b * sin_a, sin_b * sin_a, cos_a);
        glVertex3d (cos_b * r2,    sin_b * r2,    h);
        glVertex3d (cos_b * r1,    sin_b * r1,    0);
      }
      glEnd ();

      if (closed)
      {
        glPushMatrix ();
        glTranslated (0.0, 0.0, h);
        //gluDisk (cylinder, 0, r2, 16, 1);
        Primitives::Disk (r2);
        glPopMatrix ();
      }

      //gluDeleteQuadric (cylinder);
    }

    void Primitives::Cone (const Math::pR3 & p1, double r1, const Math::pR3 & p2, double r2, bool closed)
    {
      double dx = p2.x - p1.x;
      double dy = p2.y - p1.y;
      double dz = p2.z - p1.z;
      double h = sqrt (dx*dx + dy*dy + dz*dz);
      if (h > 0.0)
      {
        double ax = 180.0 * acos (dz / h) / M_PI;
        if (dz <= 0.0) ax = -ax;
        double rx = -dy * dz;
        double ry = +dx * dz;
        glPushMatrix ();
        Translate (p1);
        glRotated (ax, rx, ry, 0.0);
        Primitives::Cone (h, r1, r2, closed);
        glPopMatrix();
      }
    }

    void Primitives::Cylinder (double h, double r, bool closed)
    {
      Primitives::Cone (h, r, r, closed);
    }

    void Primitives::Cylinder (const Math::pR3 & p1, const Math::pR3 & p2, double r, bool closed)
    {
      Primitives::Cone (p1, r, p2, r, closed);
    }

  } // namespace GL
} // namespace Aa

