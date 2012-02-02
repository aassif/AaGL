#ifndef __AA_GLSL__
#define __AA_GLSL__

#include <string>
#include <vector>
#include <AaException>
//#include "AaGL"

namespace Aa
{
  namespace GL
  {
    class ShaderException : public std::runtime_error
    {
      public:
        static ShaderException Compilation (const std::string & log);
        static ShaderException Linkage     (const std::string & log);
        static ShaderException Validation  (const std::string & log);
        static ShaderException Uniform     (const char * name);

      public:
        ShaderException (const std::string & msg);
        virtual ~ShaderException () throw ();
    };

    class Program
    {
      private:
        GLuint m_id;
        
      public:
        static std::vector<std::string> ReadSource (const std::string & path) throw (FileNotFound);
        static GLuint CreateShader (GLenum type, const std::string & path) throw (FileNotFound, ShaderException);
        static std::string ShaderLog (GLuint shader);
        static std::string ProgramLog (GLuint program);

      public:
        // Variables.
        template <typename T> static inline void SetUniform (int location, T v1);
        template <typename T> static inline void SetUniform (int location, T v1, T v2);
        template <typename T> static inline void SetUniform (int location, T v1, T v2, T v3);
        template <typename T> static inline void SetUniform (int location, T v1, T v2, T v3, T v4);
        // Arrays.
        template <typename T, int N> static inline void SetUniform (int location, int count, const T * values);
        // Matrices.
        template <typename T, int N, int M> static inline void SetUniform (int location, const T * values, int count = 1);

      public:
        Program ();
        virtual ~Program ();
        inline GLuint id () const {return m_id;}
        void attach (GLenum type, const std::string & path) throw (FileNotFound, ShaderException);
        void validate () const throw (ShaderException);
        void link () const throw (ShaderException);
        void use () const;
        GLint location (const char * name) const;
        // Variables.
        template <typename T> inline void set (const char * name, T v1) const throw (ShaderException);
        template <typename T> inline void set (const char * name, T v1, T v2) const throw (ShaderException);
        template <typename T> inline void set (const char * name, T v1, T v2, T v3) const throw (ShaderException);
        template <typename T> inline void set (const char * name, T v1, T v2, T v3, T v4) const throw (ShaderException);
        // Arrays.
        template <typename T, int N> inline void set (const char * name, int count, const T * values) const throw (ShaderException);
        // Matrices.
        template <typename T, int N, int M> inline void set (const char * name, const T * values, int count = 1) const throw (ShaderException);
    };

#define AA_GLSL_UNIFORM1(T, F) \
    template <>\
    inline void Program::SetUniform<T> (int location, T v1) {F (location, v1);}

    AA_GLSL_UNIFORM1 (GLint,   glUniform1i)
    AA_GLSL_UNIFORM1 (GLfloat, glUniform1f)

#undef AA_GLSL_UNIFORM1

    template <typename T>
    inline void Program::set (const char * name, T v1) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, v1);
    }

#define AA_GLSL_UNIFORM2(T, F) \
    template <>\
    inline void Program::SetUniform<T> (int location, T v1, T v2) {F (location, v1, v2);}

    AA_GLSL_UNIFORM2 (GLint,   glUniform2i)
    AA_GLSL_UNIFORM2 (GLfloat, glUniform2f)

#undef AA_GLSL_UNIFORM2

    template <typename T>
    inline void Program::set (const char * name, T v1, T v2) const
      throw (ShaderException)
    {
      GLint l = glGetUniformLocation (m_id, name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, v1, v2);
    }

#define AA_GLSL_UNIFORM3(T, F) \
    template <>\
    inline void Program::SetUniform<T> (int location, T v1, T v2, T v3) {F (location, v1, v2, v3);}

    AA_GLSL_UNIFORM3 (GLint,   glUniform3i)
    AA_GLSL_UNIFORM3 (GLfloat, glUniform3f)

#undef AA_GLSL_UNIFORM3

    template <typename T>
    inline void Program::set (const char * name, T v1, T v2, T v3) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, v1, v2, v3);
    }

#define AA_GLSL_UNIFORM4(T, F) \
    template <>\
    inline void Program::SetUniform<T> (int location, T v1, T v2, T v3, T v4) {F (location, v1, v2, v3, v4);}

    AA_GLSL_UNIFORM4 (GLint,   glUniform4i)
    AA_GLSL_UNIFORM4 (GLfloat, glUniform4f)

#undef AA_GLSL_UNIFORM4

    template <typename T>
    inline void Program::set (const char * name, T v1, T v2, T v3, T v4) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, v1, v2, v3, v4);
    }

#define AA_GLSL_UNIFORM_ARRAY(T, N, F) \
    template <>\
    inline void Program::SetUniform<T, N> (int location, int count, const T * values) {F (location, count, values);}

    AA_GLSL_UNIFORM_ARRAY (GLint,   1, glUniform1iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 1, glUniform1fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   2, glUniform2iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 2, glUniform2fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   3, glUniform3iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 3, glUniform3fv)

    AA_GLSL_UNIFORM_ARRAY (GLint,   4, glUniform4iv)
    AA_GLSL_UNIFORM_ARRAY (GLfloat, 4, glUniform4fv)

#undef AA_GLSL_UNIFORM_ARRAY

    template <typename T, int N>
    inline void Program::set (const char * name, int count, const T * values) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, N> (l, count, values);
    }

#define AA_GLSL_UNIFORM_MATRIX(T, N, M, F) \
    template <>\
    inline void Program::SetUniform<T, N, M> (int location, const T * values, int count) {F (location, count, GL_FALSE, values);}

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 2, glUniformMatrix2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 3, glUniformMatrix2x3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 2, 4, glUniformMatrix2x4fv)

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 2, glUniformMatrix3x2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 3, glUniformMatrix3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 3, 4, glUniformMatrix3x4fv)

    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 2, glUniformMatrix4x2fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 3, glUniformMatrix4x3fv)
    AA_GLSL_UNIFORM_MATRIX (GLfloat, 4, 4, glUniformMatrix4fv)

#undef AA_GLSL_UNIFORM_MATRIX

    template <typename T, int N, int M>
    inline void Program::set (const char * name, const T * values, int count) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, N, M> (l, values, count);
    }

  } // namespace GL
} // namespace Aa

#endif // __AA_GLSL__

