#ifndef AA_GL_INCLUDE_RESOLVER__H
#define AA_GL_INCLUDE_RESOLVER__H

#include <map>
#include <set>
#include <iomanip>
#include <AaText>

namespace Aa
{
  namespace GLSL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::IncludeResolver /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class IncludeResolver
    {
      private:
        inline static
        std::string ParseIncludeName (std::istream & is, char delim1, char delim2)
          throw (Aa::ParseError)
        {
          using namespace TextParsing;

          is >>= delim1;
          std::string id;
          for (char c = is.get (); c != delim2; c = is.get ())
          {
            if (c == -1) throw ParseError::Value (delim2);
            id += c;
          }
          return id;
        }

        inline static
        std::string ParseIncludeName (std::istream & is)
          throw (Aa::ParseError)
        {
          int c = is.peek ();
          switch (c)
          {
            case '"': return ParseIncludeName (is, '"', '"');
            case '<': return ParseIncludeName (is, '<', '>');
            case -1 : throw Aa::ParseError::Type ("<id>");
            default : throw Aa::ParseError::Type ("<id>", c);
          }
        }

        inline static
        std::string ParseInclude (std::istream & is)
          throw (Aa::ParseError)
        {
          using namespace TextParsing;

          is >> std::ws;
          is >>= '#';
          is >> std::ws;
          is >>= "include";
          is >> std::ws;

          return ParseIncludeName (is);
        }

        inline static
        std::string ParseExtension (std::istream & is)
          throw (Aa::ParseError)
        {
          using namespace TextParsing;

          is >> std::ws;
          is >>= '#';
          is >> std::ws;
          is >>= "extension";
          std::string extension;
          is >> extension >> std::ws;
          is >>= ':';
          std::string behaviour;
          is >> behaviour >> std::ws;

          return extension;
        }

        inline static
        bool FilterExtension (std::string & line)
        {
          try
          {
            std::istringstream iss (line);
            std::string extension = ParseExtension (iss);
            std::cout << "#extension " << extension << std::endl;
            return extension == "GL_ARB_shading_language_include";
          }
          catch (...)
          {
            return false;
          }
        }

      private:
        std::map<std::string, std::string> m_includes;

      private:
        inline
        std::string resolve (const std::string     & id,
                             std::set<std::string> * skip) const
        {
          if (skip->find (id) != skip->end ())
            return std::string ();

          skip->insert (id);

          std::string source = (*this) [id];
          std::string destination;

          std::istringstream iss1 (source);
          for (std::string line; getline (iss1, line, '\n');)
          {
            if (! FilterExtension (line))
            {
              try
              {
                std::istringstream iss2 (line);
                std::string child = ParseInclude (iss2);
                destination += this->resolve (child, skip);
                while (iss2.peek () != -1) destination += iss2.get ();
              }
              catch (Aa::ParseError & e)
              {
                destination += line;
              }
              destination += '\n';
            }
          }

          return destination;
        }

      public:
        inline
        IncludeResolver () :
          m_includes ()
        {
        }

        inline
        void set (const std::string & id, const std::string & source)
        {
          m_includes [id] = source;
        }

        inline
        const std::string & operator[] (const std::string & id) const
          throw (std::out_of_range)
        {
          std::map<std::string, std::string>::const_iterator i = m_includes.find (id);
          if (i == m_includes.end ()) throw std::out_of_range ("Aa::GL::IncludeResolver: " + id);
          return i->second;
        }

        inline
        std::string operator() (const std::string & id) const
        {
          std::set<std::string> skip;
          return this->resolve (id, &skip);
        }
    };

  } // namespace GLSL
} // namespace Aa

#endif // AA_GL_INCLUDE_RESOLVER__H

