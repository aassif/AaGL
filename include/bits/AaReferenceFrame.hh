#ifndef __AA_GL_REFERENCE_FRAME__
#define __AA_GL_REFERENCE_FRAME__

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::ReferenceFrame //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class ReferenceFrame :
      public GL::Plugin
    {
      public:
        void glDraw (bool)
        {
          glDisable (GL_LIGHTING);
          glBegin (GL_LINES);
          glColor4f (1.0, 0.0, 0.0, 1.0); glVertex3d (0.0, 0.0, 0.0); glVertex3d (1.0, 0.0, 0.0);
          glColor4f (0.0, 1.0, 0.0, 1.0); glVertex3d (0.0, 0.0, 0.0); glVertex3d (0.0, 1.0, 0.0);
          glColor4f (0.0, 0.0, 1.0, 1.0); glVertex3d (0.0, 0.0, 0.0); glVertex3d (0.0, 0.0, 1.0);
          glEnd ();
        }
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL_REFERENCE_FRAME__

