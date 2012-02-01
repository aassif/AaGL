#ifndef __AA_GLUT_WATERMARK__
#define __AA_GLUT_WATERMARK__

#include <string>

namespace Aa
{
  namespace GL
  {
    class AA_GL_API Watermark :
      public Aa::GL::Plugin
    {
      protected:
        bool m_enabled;
        std::string m_text;
        float m_color [4];

      public:
        // Constructor.
        Watermark (const std::string & text = "");
        // Enable / disable.
        inline void setEnabled (bool e) {m_enabled = e;}
        inline bool isEnabled () const {return m_enabled;}
        // Signature.
        inline void setText (const std::string & text) {m_text = text;}
        inline const std::string & getText () const {return m_text;}
        // OpenGL.
        void glDraw (bool);
    };

    Watermark::Watermark (const std::string & text) :
      m_enabled (true), m_text (text), m_color ()
    {
      static const float DEFAULT_COLOR [] = {0.0, 0.0, 0.0, 0.5};
      std::copy (DEFAULT_COLOR, DEFAULT_COLOR + 4, m_color);
    }

    void Watermark::glDraw (bool)
    {
      if (m_enabled)
      {
        GLint viewport [4];
        glGetIntegerv (GL_VIEWPORT, viewport);

        glMatrixMode (GL_PROJECTION);
        glPushMatrix ();
        glLoadIdentity ();
        glOrtho (0.0, viewport [2], 0.0, viewport [3], 0.0, 1.0);

        glMatrixMode (GL_MODELVIEW);
        glPushMatrix ();
        glLoadIdentity ();

        glDisable (GL_LIGHTING);
        glEnable (GL_BLEND);
        glColor4fv (m_color);
        Aa::GL::Text::Write (8, 8, m_text);

        glMatrixMode (GL_PROJECTION);
        glPopMatrix ();

        glMatrixMode (GL_MODELVIEW);
        glPopMatrix ();
      }
    }

  } // namespace GL
} // namespace Aa

#endif // __AA_GLUT_WATERMARK__

