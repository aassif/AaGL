#include <fstream>
#include <sstream>
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

    vector<string> Program::ReadSource (const string & path)
      throw (FileNotFound)
    {
      ifstream ifs (path.c_str ());
      if (! ifs.is_open ())
        throw FileNotFound (path);

      vector<string> lines;
      for (string line; getline (ifs, line);)
        lines.push_back (line + '\n');

      return lines;
    }

    string Program::ShaderLog (GLuint shader)
    {
      GLint n = 0;
      glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &n);
      GLchar * strLog = new GLchar [n];
      glGetShaderInfoLog (shader, n, NULL, strLog);
      string log (strLog);
      delete[] strLog;
      return log;
    }

    string Program::ProgramLog (GLuint program)
    {
      GLint n = 0;
      glGetProgramiv (program, GL_INFO_LOG_LENGTH, &n);
      GLchar * strLog = new GLchar [n];
      glGetProgramInfoLog (program, n, NULL, strLog);
      string log (strLog);
      delete[] strLog;
      return log;
    }

    GLuint Program::CreateShader (GLenum type, const string & path)
      throw (FileNotFound, ShaderException)
    {
      GLuint shader = glCreateShader (type);

      cout << path << endl;
      vector<string> source = ReadSource (path);
      unsigned int n = source.size ();
      //cout << "n = " << n << endl;
      vector<const GLchar *> lines (n);
      vector<GLint> lengths (n);
      for (unsigned int i = 0; i < n; ++i)
      {
        const string line = source [i];
        lines   [i] = line.c_str ();
        lengths [i] = line.size ();
      }

      glShaderSource (shader, n, &(lines[0]), &(lengths[0]));

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
      GLuint shader = CreateShader (type, path);
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

    GLint Program::location (const char * name) const
    {
      return glGetUniformLocation (m_id, name);
    }

    void Program::use () const
    {
      glUseProgram (m_id);
    }

  } // namespace GL
} // namespace Aa

