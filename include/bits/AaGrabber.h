#ifndef __AA_GL_GRABBER__
#define __AA_GL_GRABBER__

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Grabber /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Grabber
    {
      protected:
        unsigned int    m_width;
        unsigned int    m_height;
        unsigned char * m_buffer1;
        unsigned char * m_buffer2;

      public:
        // Constructor.
        Grabber (unsigned int w, unsigned int h);

        // Destructor.
        virtual ~Grabber ();

        // Grab.
        virtual void glGrab ();
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL_GRABBER__

