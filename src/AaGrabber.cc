#include "AaGrabber"

using namespace std;

// Aa::GL //////////////////////////////////////////////////////////////////////

namespace Aa
{

// Aa::GL //////////////////////////////////////////////////////////////////////

  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Grabber /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Grabber::Grabber (unsigned int w, unsigned int h) :
      m_width (std::max (1u, w)), m_height (std::max (1u, h))
    {
      m_buffer1 = new unsigned char [m_width * m_height * 4];
      m_buffer2 = new unsigned char [m_width * m_height * 4];
    }

    Grabber::~Grabber ()
    {
      delete[] m_buffer1;
      delete[] m_buffer2;
    }

    void Grabber::glGrab ()
    {
      // Grab buffer.
      glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
      glReadPixels (0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_buffer1);

      // Flip buffer vertically.
      for (unsigned char
             * i0 = m_buffer1,
             * i1 = m_buffer1 + m_width * 3,
             * j0 = m_buffer2 + m_width * m_height * 3,
             * e0 = m_buffer1 + m_width * m_height * 3;
           i0 != e0;
           i0 = i1, i1 += m_width * 3, j0 -= m_width * 3)
        std::copy (i0, i1, j0);
    }

  } // namespace GL

} // namespace Aa

