#include <sstream>
#include "AaExtensions"

using namespace std;

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Extensions //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    bool Extensions::m_detected = false;
    set<string> Extensions::m_extensions;

    void Extensions::detect ()
    {
      istringstream i ((const char *) glGetString (GL_EXTENSIONS));
      for (string ext; i >> ext;) m_extensions.insert (ext);
      m_detected = true;
    }

    bool Extensions::supports (const string & ext)
    {
      if (! m_detected) Extensions::detect ();
      return m_extensions.find (ext) != m_extensions.end ();
    }

    void Extensions::print (ostream & o)
    {
      if (! m_detected) Extensions::detect ();

      o << "Detected extensions:\n";
      if (m_extensions.empty ())
        o << "(none)\n";
      else
        {
          set<string>::const_iterator i = m_extensions.begin ();
          o << *(i++);
          for (set<string>::const_iterator e = m_extensions.end (); i != e;)
            o << ", " << *(i++);
          o << ".\n";
        }
    }

    void Extensions::print (ostream & o, const string & ext)
    {
      bool b = Extensions::supports (ext);
      o << ext << ": " << (b ? "supported" : "not supported");
    }

  } // namespace GL
} // namespace Aa

