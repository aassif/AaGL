#ifndef __AA_GL_EXTENSIONS__
#define __AA_GL_EXTENSIONS__

#include <set>
#include <string>
#include <iostream>

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Extensions //////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Extensions
    {
      private:
        static bool m_detected;
        static std::set<std::string> m_extensions;

        // Detection of the extensions.
        static void detect ();

      public:
        // Tests if an extension is supported.
        static bool supports (const std::string &);
        // Prints all the detected extensions.
        static void print (std::ostream &);
        // Prints whether an extension is detected or not.
        static void print (std::ostream &, const std::string &);
    };

  }
}

#endif // __AA_GL_EXTENSIONS__

