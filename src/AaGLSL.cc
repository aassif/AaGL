#include <fstream>
#include <sstream>
#include <iterator>
#include "AaGLSL"

using namespace std;

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::ShaderException /////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    ShaderException ShaderException::Compilation (const string & log)
    {
      ostringstream oss;
      oss << "GL_COMPILE_STATUS: " << endl << log << endl;
      return ShaderException (oss.str ());
    }

    ShaderException ShaderException::Linkage (const string & log)
    {
      ostringstream oss;
      oss << "GL_LINK_STATUS: " << endl << log << endl;
      return ShaderException (oss.str ());
    }

    ShaderException ShaderException::Validation (const string & log)
    {
      ostringstream oss;
      oss << "GL_VALIDATION_STATUS: " << endl << log << endl;
      return ShaderException (oss.str ());
    }

    ShaderException ShaderException::Uniform (const char * name)
    {
      ostringstream oss;
      oss << "glUniform: can't find uniform '" << name << "'";
      return ShaderException (oss.str ());
    }

    ShaderException::ShaderException (const string & message) :
      runtime_error (message)
    {
    }

    ShaderException::~ShaderException () throw ()
    {
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Program /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    string Program::ReadSource (const string & path)
      throw (FileNotFound)
    {
      ifstream ifs (path.c_str ());

      if (! ifs.is_open ())
        throw FileNotFound (path);

      return string (istreambuf_iterator<char> (ifs), istreambuf_iterator<char> ());
    }

    string Program::ShaderLog (GLuint shader)
    {
      GLint n = 0;
      glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &n);
      GLchar * strLog = new GLchar [n];
      glGetShaderInfoLog (shader, n, NULL, strLog);
      string log (strLog, n);
      delete[] strLog;
      return log;
    }

    string Program::ProgramLog (GLuint program)
    {
      GLint n = 0;
      glGetProgramiv (program, GL_INFO_LOG_LENGTH, &n);
      GLchar * strLog = new GLchar [n];
      glGetProgramInfoLog (program, n, NULL, strLog);
      string log (strLog, n);
      delete[] strLog;
      return log;
    }

    GLuint Program::CreateShader (GLenum type,
                                  GLsizei count,
                                  const GLchar ** buffers,
                                  const GLint * lengths)
      throw (FileNotFound, ShaderException)
    {
      GLuint shader = glCreateShader (type);
      glShaderSource (shader, count, buffers, lengths);
      glCompileShader (shader);

      GLint okay = 0;
      glGetShaderiv (shader, GL_COMPILE_STATUS, &okay);
      if (! okay)
      {
        string log = ShaderLog (shader);
        throw ShaderException::Compilation (log);
      }

      return shader;
    }

    GLuint Program::CreateShader (GLenum type,
                                  const GLchar * buffer,
                                  GLint length)
      throw (FileNotFound, ShaderException)
    {
      return CreateShader (type, 1, &buffer, &length);
    }

    Program::Program () :
      m_id (0)
    {
      m_id = glCreateProgram ();
    }

    Program::~Program ()
    {
      glDeleteProgram (m_id);
    }

    void Program::attach (GLenum type, const string & path)
      throw (FileNotFound, ShaderException)
    {
      string source = ReadSource (path);
      GLuint shader = CreateShader (type, source.c_str (), source.size ());
      glAttachShader (m_id, shader);
      glDeleteShader (shader);
    }

    void Program::link () const throw (ShaderException)
    {
      glLinkProgram (m_id);

      GLint okay = 0;
      glGetProgramiv (m_id, GL_LINK_STATUS, &okay);
      if (! okay)
      {
        string log = ProgramLog (m_id);
        cout << "GL_LINK_STATUS: " << endl << log << endl;
        throw ShaderException::Linkage (log);
      }
    }

    void Program::validate () const throw (ShaderException)
    {
      glValidateProgram (m_id);

      GLint okay = 0;
      glGetProgramiv (m_id, GL_VALIDATE_STATUS, &okay);
      if (! okay)
      {
        string log = ProgramLog (m_id);
        cout << "GL_VALIDATE_STATUS: " << endl << log << endl;
        throw ShaderException::Validation (log);
      }
    }

    GLint Program::location (const GLchar * name) const
    {
      return glGetUniformLocation (m_id, name);
    }

    void Program::use () const
    {
      glUseProgram (m_id);
    }

  } // namespace GL
} // namespace Aa

