#ifndef __AA_GLSL__
#define __AA_GLSL__

#include <string>
#include <vector>
#include <AaException>

namespace Aa
{
  namespace GL
  {
    class AA_GL_API ShaderException : public std::runtime_error
    {
      public:
        static ShaderException Compilation (const std::string & log);
        static ShaderException Linkage     (const std::string & log);
        static ShaderException Validation  (const std::string & log);
        static ShaderException Uniform     (const char * name);

      public:
        ShaderException (const std::string & message);
        virtual ~ShaderException () throw ();
    };

    class AA_GL_API Program
    {
      private:
        GLuint m_id;
        
      public:
        inline static
        std::string ReadSource (const std::string & path)
          throw (FileNotFound);

        inline static
        GLuint CreateShader (GLenum type,
                             GLsizei count,
                             const GLchar ** buffers,
                             const GLint * lengths)
          throw (FileNotFound, ShaderException);

        inline static
        GLuint CreateShader (GLenum type,
                             const GLchar * buffer,
                             GLint length)
          throw (FileNotFound, ShaderException);

        inline static
        std::string ShaderLog (GLuint shader);

        inline static
        std::string ProgramLog (GLuint program);

      public:
        // Variables.
        template <typename T>
        inline static
        void SetUniform (GLint location, T t);

        template <typename T, unsigned int m>
        inline static
        void SetUniform (GLint location, const V<T, m> & v);

        template <typename T, unsigned int m, unsigned int n>
        inline static
        void SetUniform (GLint location, const M<T, m, n> & matrix);

        // Arrays.
        template <typename T>
        inline static
        void SetUniform (GLint location, GLsizei count, const T * values);

        template <typename T, unsigned int m>
        inline static
        void SetUniform (GLint location, GLsizei count, const V<T, m> * values);

        template <typename T, unsigned int m, unsigned int n>
        inline static
        void SetUniform (GLint location, GLsizei count, const M<T, m, n> * values);

      public:
        Program ();
        virtual ~Program ();
        inline GLuint id () const {return m_id;}

        // Shader attachment.
        void attach (GLenum type,
                     const std::string & path)
          throw (FileNotFound, ShaderException);

        // Validation, linkage and usage.
        void validate () const throw (ShaderException);
        void link () const throw (ShaderException);
        void use () const;

        // Location.
        GLint location (const char * name) const;

        // Variables.
        template <typename T>
        inline
        void set (const char * name, T t) const
          throw (ShaderException);

        template <typename T, unsigned int m>
        inline
        void set (const char * name, const V<T, m> & v) const
          throw (ShaderException);

        template <typename T, unsigned int m, unsigned int n>
        inline
        void set (const char * name, const M<T, m, n> & matrix) const
          throw (ShaderException);

        // Arrays.
        template <typename T>
        inline
        void set (const char * name, GLsizei count, const T * values) const
          throw (ShaderException);

        template <typename T, unsigned int m>
        inline
        void set (const char * name, GLsizei count, const V<T, m> * values) const
          throw (ShaderException);

        template <typename T, unsigned int m, unsigned int n>
        inline
        void set (const char * name, GLsizei count, const M<T, m, n> * values) const
          throw (ShaderException);
    };

#define AA_GLSL_UNIFORM_VALUE(T, F) \
    template <>\
    inline\
    void Program::SetUniform<T> (GLint location, T t)\
    {\
      F (location, t);\
    }

    AA_GLSL_UNIFORM_VALUE (GLint,   glUniform1i)
    AA_GLSL_UNIFORM_VALUE (GLuint,  glUniform1ui)
    AA_GLSL_UNIFORM_VALUE (GLfloat, glUniform1f)

#undef AA_GLSL_UNIFORM_VALUE

    template <typename T>
    inline
    void Program::set (const char * name, T t) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, t);
    }

#define AA_GLSL_UNIFORM_VECTOR(T, m, F) \
    template <>\
    inline\
    void Program::SetUniform<T, m> (GLint location, const V<T, m> & v)\
    {\
      F (location, 1, &(v[0]));\
    }

    AA_GLSL_UNIFORM_VECTOR (GLint,   2, glUniform2iv)
    AA_GLSL_UNIFORM_VECTOR (GLuint,  2, glUniform2uiv)
    AA_GLSL_UNIFORM_VECTOR (GLfloat, 2, glUniform2fv)

    AA_GLSL_UNIFORM_VECTOR (GLint,   3, glUniform3iv)
    AA_GLSL_UNIFORM_VECTOR (GLuint,  3, glUniform3uiv)
    AA_GLSL_UNIFORM_VECTOR (GLfloat, 3, glUniform3fv)

    AA_GLSL_UNIFORM_VECTOR (GLint,   4, glUniform4iv)
    AA_GLSL_UNIFORM_VECTOR (GLuint,  4, glUniform4uiv)
    AA_GLSL_UNIFORM_VECTOR (GLfloat, 4, glUniform4fv)

#undef AA_GLSL_UNIFORM_VECTOR

    template <typename T, unsigned int m>
    inline
    void Program::set (const char * name, const V<T, m> & v) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, m> (l, v);
    }

#define AA_GLSL_UNIFORM_MATRIX(T, m, n, F) \
    template <>\
    inline\
    void Program::SetUniform<T, m, n> (GLint location, const M<T, m, n> & matrix)\
    {\
      F (location, 1, GL_FALSE, &(matrix[0][0]));\
    }

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

    template <typename T, unsigned int m, unsigned int n>
    inline
    void Program::set (const char * name, const M<T, m, n> & matrix) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, m, n> (l, matrix);
    }

#define AA_GLSL_UNIFORM_VALUE_ARRAY(T, F) \
    template <>\
    inline\
    void Program::SetUniform<T> (GLint location, GLsizei count, const T * values)\
    {\
      F (location, count, values);\
    }

    AA_GLSL_UNIFORM_VALUE_ARRAY (GLint,   glUniform1iv)
    AA_GLSL_UNIFORM_VALUE_ARRAY (GLuint,  glUniform1uiv)
    AA_GLSL_UNIFORM_VALUE_ARRAY (GLfloat, glUniform1fv)

#undef AA_GLSL_UNIFORM_VALUE_ARRAY

    template <typename T>
    inline
    void Program::set (const char * name, GLsizei count, const T * values) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T> (l, count, values);
    }

#define AA_GLSL_UNIFORM_VECTOR_ARRAY(T, m, F) \
    template <>\
    inline\
    void Program::SetUniform<T, m> (GLint location, GLsizei count, const V<T, m> * values)\
    {\
      F (location, count, &(values[0][0]));\
    }

    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLint,   2, glUniform2iv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLuint,  2, glUniform2uiv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLfloat, 2, glUniform2fv)

    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLint,   3, glUniform3iv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLuint,  3, glUniform3uiv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLfloat, 3, glUniform3fv)

    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLint,   4, glUniform4iv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLuint,  4, glUniform4uiv)
    AA_GLSL_UNIFORM_VECTOR_ARRAY (GLfloat, 4, glUniform4fv)

#undef AA_GLSL_UNIFORM_VECTOR_ARRAY

    template <typename T, unsigned int m>
    inline
    void Program::set (const char * name, GLsizei count, const V<T, m> * values) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, m> (l, count, values);
    }

#define AA_GLSL_UNIFORM_MATRIX_ARRAY(T, m, n, F) \
    template <>\
    inline\
    void Program::SetUniform<T, m, n> (GLint location, GLsizei count, const M<T, m, n> * values)\
    {\
      F (location, count, GL_FALSE, &(values[0][0][0]));\
    }

    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 2, 2, glUniformMatrix2fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 2, 3, glUniformMatrix2x3fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 2, 4, glUniformMatrix2x4fv)

    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 3, 2, glUniformMatrix3x2fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 3, 3, glUniformMatrix3fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 3, 4, glUniformMatrix3x4fv)

    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 4, 2, glUniformMatrix4x2fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 4, 3, glUniformMatrix4x3fv)
    AA_GLSL_UNIFORM_MATRIX_ARRAY (GLfloat, 4, 4, glUniformMatrix4fv)

#undef AA_GLSL_UNIFORM_MATRIX_ARRAY

    template <typename T, unsigned int m, unsigned int n>
    inline
    void Program::set (const char * name, GLsizei count, const M<T, m, n> * values) const
      throw (ShaderException)
    {
      GLint l = this->location (name);
      if (l == -1) throw ShaderException::Uniform (name);
      SetUniform<T, m, n> (l, count, values);
    }

  } // namespace GL
} // namespace Aa

#endif // __AA_GLSL__

