#ifndef __AA_GLUT_TEXT__
#define __AA_GLUT_TEXT__

#include <GL/glut.h>

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Text ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Text
    {
      public:
        inline static int Width (char, void * font = GLUT_BITMAP_HELVETICA_18);
        inline static int Width (const std::string &, void * font = GLUT_BITMAP_HELVETICA_18);
        inline static void Write (char, void * font = GLUT_BITMAP_HELVETICA_18);
        inline static void Write (const std::string &, void * font = GLUT_BITMAP_HELVETICA_18);
        inline static void Write (int x, int y, const std::string &, void * font = GLUT_BITMAP_HELVETICA_18);
    };

    
////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Text ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    int Text::Width (char c, void * font)
    {
      return glutBitmapWidth (font, c);
    }
    
    int Text::Width (const std::string & s, void * font)
    {
      int r = 0;
      for (std::string::const_iterator
             i = s.begin (), e = s.end (); i != e;) r += Text::Width (*(i++), font);
      return r;
    }
    
    void Text::Write (char c, void * font)
    {
      glutBitmapCharacter (font, c);
    }

    void Text::Write (const std::string & s, void * font)
    {
      for (std::string::const_iterator
             i = s.begin (), e = s.end (); i != e;) Text::Write (*(i++), font);
    }

    void Text::Write (int x, int y, const std::string & s, void * font)
    {
      glRasterPos2i (x, y);
      /*float rasterColor [4];
      glGetFloatv (GL_CURRENT_RASTER_COLOR, rasterColor);
      cout << "CurrentRasterColor = "
           << rasterColor [0] << ", " << rasterColor [1] << ", "
           << rasterColor [2] << ", " << rasterColor [2] << endl;*/
      Text::Write (s, font);
    }
    
  } // namespace GL
} // namespace Aa

#endif // __AA_GLUT_TEXT__

