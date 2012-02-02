#ifndef __AA_GL_FFMPEG__
#define __AA_GL_FFMPEG__

#include <AaFFmpeg>
//#include "AaGrabber"

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::GrabberFFmpeg ///////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class AA_GL_API GrabberFFmpeg : public Grabber
    {
      protected:
        Aa::FFmpeg::Encoder * m_encoder;

      public:
        // Constructor.
        GrabberFFmpeg (Aa::FFmpeg::Encoder * encoder) :
          Grabber (encoder->width (), encoder->height ()),
          m_encoder (encoder)
        {
        }

        // Destructor.
        virtual ~GrabberFFmpeg ()
        {
        }

        // Encoder.
        Aa::FFmpeg::Encoder * encoder () {return m_encoder;}

        // Grab.
        virtual void glGrab () //throw (FFmpegException)
        {
          Grabber::glGrab ();
          try
          {
            m_encoder->encode (m_buffer2);
          }
          catch (Aa::FFmpeg::FFmpegException & ex)
          {
            std::cerr << ex.what () << std::endl;
          }
        }
    };

  } // namespace GL
} // namespace Aa

#endif // __AA_GL_FFMPEG__

