#ifndef AA_GLSL_SUBROUTINE__H
#define AA_GLSL_SUBROUTINE__H

#include <vector>
#include <map>

namespace Aa
{
  namespace GLSL
  {

    template <AaUInt n>
    class SubroutineManager
    {
      public:
        typedef const char * Name;
        typedef std::vector<Name> NameList;
        typedef std::map<Name, NameList> Config;

      private:
        class Subroutine
        {
          private:
            GLint               m_location;
            std::vector<GLuint> m_indices;

          public:
            Subroutine (GLuint id,
                        GLenum type, Name name,
                        const NameList & v) :
              m_location (),
              m_indices ()
            {
              m_location = glGetSubroutineUniformLocation (id, type, name);

              for (auto i = v.begin (); i != v.end ();)
                m_indices.push_back (glGetSubroutineIndex (id, type, *(i++)));
            }

            inline
            GLint location () const
            {
              return m_location;
            }

            inline
            GLuint operator[] (AaUInt k) const
            {
              return (k < m_indices.size () ? m_indices [k] : GL_INVALID_INDEX);
            }
        };

      private:
        GLenum                  m_type;
        std::vector<Subroutine> m_subroutines;
        GLuint                  m_indices [n];

      public:
        inline
        SubroutineManager (Program * p, GLenum type, const Config & config) :
          m_type (type),
          m_subroutines (),
          m_indices ()
        {
          GLint active = 0;
          glGetProgramStageiv (p->id (), m_type, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &active);
          if (active != n)
            throw std::runtime_error ("SubroutineManager: active != n");

          for (auto i = config.begin (); i != config.end (); ++i)
            m_subroutines.push_back (Subroutine (p->id (), type, i->first, i->second));

          if (m_subroutines.size () != n)
            throw std::runtime_error ("SubroutineManager: config.size != n");
        }

        inline
        void set (const AaUInt (& values) [n])
        {
          for (AaUInt i = 0; i < n; ++i)
          {
            const Subroutine & s = m_subroutines [i];

            GLint j = s.location ();
            if (j >= 0 && (GLuint) j < n) m_indices [j] = s [values [i]];
          }

          glUniformSubroutinesuiv (m_type, n, m_indices);
        }
    };

  }
}

#endif // AA_GLSL_SUBROUTINE__H

