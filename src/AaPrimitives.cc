#define _USE_MATH_DEFINES // Win32
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
      //GLboolean backup_glBlend = glIsEnabled (GL_BLEND);
      //glEnable (GL_BLEND);
      // Smooth lines.
      //GLboolean backup_glLineSmooth = glIsEnabled (GL_LINE_SMOOTH);
      //glEnable (GL_LINE_SMOOTH);
      //glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
      // Depth mask.
      //GLboolean backup_glDepthMask;
      //glGetBooleanv (GL_DEPTH_WRITEMASK, &backup_glDepthMask);
      //glDepthMask (GL_FALSE);
      // Polygon mode.
      //GLint backup_glPolygonMode [2];
      //glGetIntegerv (GL_POLYGON_MODE, backup_glPolygonMode);
      //glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

      // Bounding box vertices.
      static const GLfloat BOX_VERTICES [] = {
        0, 0, 0,
        1, 0, 0,
        0, 1, 0,
        1, 1, 0,
        0, 0, 1,
        1, 0, 1,
        0, 1, 1,
        1, 1, 1
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
      glVertexPointer (3, GL_FLOAT, 0, BOX_VERTICES);
      glDrawElements (GL_QUADS, 24, GL_UNSIGNED_BYTE, BOX_FACES);
      glDisableClientState (GL_VERTEX_ARRAY);

      //glPolygonMode (GL_FRONT, backup_glPolygonMode [0]);
      //glPolygonMode (GL_BACK,  backup_glPolygonMode [1]);
      //if (backup_glDepthMask) glDepthMask (GL_TRUE);
      //if (! backup_glLineSmooth) glDisable (GL_LINE_SMOOTH);
      //if (! backup_glBlend)      glDisable (GL_BLEND);
    }

    void Primitives::Box (const box3 & b)
    {
      glPushMatrix ();
      GL::Translate (b.pos  ());
      GL::Scale     (b.dims ());
      Primitives::Box ();
      glPopMatrix ();
    }

#if 0
    const GLfloat Primitives::CONTOUR [][2] = {
      {+1.000f,  0.000f},
      {+0.966f, +0.259f},
      {+0.866f, +0.500f},
      {+0.707f, +0.707f},
      {+0.500f, +0.866f},
      {+0.259f, +0.966f},
      { 0.000f, +1.000f},
      {-0.259f, +0.966f},
      {-0.500f, +0.866f},
      {-0.707f, +0.707f},
      {-0.866f, +0.500f},
      {-0.966f, +0.259f},
      {-1.000f,  0.000f},
      {-0.966f, -0.259f},
      {-0.866f, -0.500f},
      {-0.707f, -0.707f},
      {-0.500f, -0.866f},
      {-0.259f, -0.966f},
      { 0.000f, -1.000f},
      {+0.259f, -0.966f},
      {+0.500f, -0.866f},
      {+0.707f, -0.707f},
      {+0.866f, -0.500f},
      {+0.966f, -0.259f},
      {+1.000f,  0.000f}
    };
#endif

    void Primitives::Disk (float r)
    {
      int nSlices = 24;

      glBegin (GL_TRIANGLE_FAN);
      glNormal3f (0, 0, 1);
      glVertex2f (0, 0);
      for (int i = 0; i <= nSlices; ++i)
      {
        float a = (2.0f * M_PI * i) / nSlices;
        glVertex2f (r * std::cos (a), r * std::sin (a));
      }
      glEnd ();
    }

    void Primitives::Sphere (float r)
    {
      int nSlices = 24;
      int nStacks = 12;

      for (int i = 0; i < nStacks; ++i)
      {
        float a1 = (2.0f * M_PI * i) / nStacks;
        float cos_a1 = std::cos (a1);
        float sin_a1 = std::sin (a1);

        float a2 = (2.0f * M_PI * (i+1)) / nStacks;
        float cos_a2 = std::cos (a2);
        float sin_a2 = std::sin (a2);

        glBegin (GL_QUAD_STRIP);
        for (int j = 0; j <= nSlices; ++j)
        {
          float b = (2.0f * M_PI * j) / nSlices;
          float cos_b = std::cos (b);
          float sin_b = std::sin (b);
          // Vertex i on 1st contour.
          glNormal3f (    cos_b * sin_a1,     sin_b * sin_a1,     cos_a1);
          glVertex3f (r * cos_b * sin_a1, r * sin_b * sin_a1, r * cos_a1);
          // Vertex i on 2nd contour.
          glNormal3f (    cos_b * sin_a2,     sin_b * sin_a2,     cos_a2);
          glVertex3f (r * cos_b * sin_a2, r * sin_b * sin_a2, r * cos_a2);
        }
        glEnd ();
      }

      //glutSolidSphere (r, nSlices, nStacks);
    }

    void Primitives::Sphere (const vec3 & c, float r)
    {
      glPushMatrix ();
      GL::Translate (c);
      Primitives::Sphere (r);
      glPopMatrix ();
    }

    void Primitives::Cone (float h, float r1, float r2, bool closed)
    {
      //static GLUquadricObj * cylinder = NULL;
      //if (cylinder == NULL) cylinder = gluNewQuadric ();

      int nSlices = 8;
      
      if (closed)
      {
        glPushMatrix ();
        glRotatef (180, 1, 0, 0); // FIXME
        //gluDisk (cylinder, 0, r1, 16, 1);
        Primitives::Disk (r1);
        glPopMatrix ();
      }

      //gluCylinder (cylinder, r1, r2, h, 16, 1);

      glBegin (GL_QUAD_STRIP);
      float a = std::atan2 (h, r2 - r1);
      float cos_a = std::cos (a);
      float sin_a = std::sin (a);
      for (int i = 0; i <= nSlices; ++i)
      {
        float b = (2.0f * M_PI * i) / nSlices;
        float cos_b = std::cos (b);
        float sin_b = std::sin (b);
        glNormal3f (cos_b * sin_a, sin_b * sin_a, cos_a);
        glVertex3f (cos_b * r2,    sin_b * r2,    h);
        glVertex3f (cos_b * r1,    sin_b * r1,    0);
      }
      glEnd ();

      if (closed)
      {
        glPushMatrix ();
        glTranslatef (0, 0, h);
        //gluDisk (cylinder, 0, r2, 16, 1);
        Primitives::Disk (r2);
        glPopMatrix ();
      }

      //gluDeleteQuadric (cylinder);
    }

    void Primitives::Cone (const vec3 & p1, float r1, const vec3 & p2, float r2, bool closed)
    {
      float dx = p2[0] - p1[0];
      float dy = p2[1] - p1[1];
      float dz = p2[2] - p1[2];
      float h = sqrt (dx*dx + dy*dy + dz*dz);
      if (h > 0)
      {
        float ax = 180 * acos (dz / h) / M_PI;
        if (dz <= 0) ax = -ax;
        float rx = -dy * dz;
        float ry = +dx * dz;
        glPushMatrix ();
        GL::Translate (p1);
        glRotatef (ax, rx, ry, 0);
        Primitives::Cone (h, r1, r2, closed);
        glPopMatrix();
      }
    }

    void Primitives::Cylinder (float h, float r, bool closed)
    {
      Primitives::Cone (h, r, r, closed);
    }

    void Primitives::Cylinder (const vec3 & p1, const vec3 & p2, float r, bool closed)
    {
      Primitives::Cone (p1, r, p2, r, closed);
    }

  } // namespace GL
} // namespace Aa

