#ifndef AA_GLSL__H
#define AA_GLSL__H

#include <string>
#include <vector>
#include <AaException>

namespace Aa
{
  namespace GLSL
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

    // Variables.
    template <typename T>
    inline
    void SetUniform (GLint location, T t);

    template <typename T, unsigned int m>
    inline
    void SetUniform (GLint location, const V<T, m> & v);

    template <typename T, unsigned int m, unsigned int n>
    inline
    void SetUniform (GLint location, const M<T, m, n> & matrix);

#if 1
    template <typename T, unsigned int m>
    inline
    void SetUniform (GLint location, const M2<T, m> & matrix)
    {
      SetUniform (location, (const M<T, m, m> &) matrix);
    }
#endif

    // Arrays.
    template <typename T>
    inline
    void SetUniform (GLint location, GLsizei count, const T * values);

    template <typename T, unsigned int m>
    inline
    void SetUniform (GLint location, GLsizei count, const V<T, m> * values);

    template <typename T, unsigned int m, unsigned int n>
    inline
    void SetUniform (GLint location, GLsizei count, const M<T, m, n> * values);

#if 1
    template <typename T, unsigned int m>
    inline
    void SetUniform (GLint location, GLsizei count, const M2<T, m> * values)
    {
      SetUniform (location, count, (const M<T, m, m> *) values);
    }
#endif

    template <typename T>
    class Location
    {
      private:
        GLint m_location;

      public:
        inline
        Location (GLint location = 0) :
          m_location (location)
        {
        }

        inline
        Location & operator= (GLint location)
        {
          m_location = location;
          return (*this);
        }

        inline
        void operator() (const T & t)
        {
          SetUniform (m_location, t);
        }
    };

    class AA_GL_API Program
    {
      private:
        GLuint m_id;
        
      public:
        /*inline*/ static
        std::string ReadSource (const std::string & path)
          AA_THROW (FileNotFound);

        /*inline*/ static
        void SetString (const std::string & name, const std::string & source);

        /*inline*/ static
        std::string String (const std::string & name);

        /*inline*/ static
        GLuint CreateShader (GLenum type,
                             GLsizei count,
                             const GLchar ** buffers,
                             const GLint * lengths)
          AA_THROW (ShaderException);

        /*inline*/ static
        GLuint CreateShader (GLenum type,
                             const GLchar * buffer,
                             GLint length)
          AA_THROW (ShaderException);

        /*inline*/ static
        std::string ShaderLog (GLuint shader);

        /*inline*/ static
        std::string ProgramLog (GLuint program);

      public:
        Program ();
        ~Program ();
        inline GLuint id () const {return m_id;}

        // Shader attachment.
        void attach (GLenum type,
                     const std::string & source)
          AA_THROW (ShaderException);

        // Validation, linkage and usage.
        void validate () const AA_THROW (ShaderException);
        void link () const AA_THROW (ShaderException);
        void use () const;

        // Location.
        GLint location (const char * name) const;
    };

#define AA_GLSL_UNIFORM_VALUE(T, F) \
    template <>\
    inline\
    void SetUniform<T> (GLint location, T t)\
    {\
      F (location, t);\
    }

    AA_GLSL_UNIFORM_VALUE (GLint,   glUniform1i)
    AA_GLSL_UNIFORM_VALUE (GLuint,  glUniform1ui)
    AA_GLSL_UNIFORM_VALUE (GLfloat, glUniform1f)

#undef AA_GLSL_UNIFORM_VALUE

#define AA_GLSL_UNIFORM_VECTOR(T, m, F) \
    template <>\
    inline\
    void SetUniform<T, m> (GLint location, const V<T, m> & v)\
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

#define AA_GLSL_UNIFORM_MATRIX(T, m, n, F) \
    template <>\
    inline\
    void SetUniform<T, m, n> (GLint location, const M<T, m, n> & matrix)\
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

#define AA_GLSL_UNIFORM_VALUE_ARRAY(T, F) \
    template <>\
    inline\
    void SetUniform<T> (GLint location, GLsizei count, const T * values)\
    {\
      F (location, count, values);\
    }

    AA_GLSL_UNIFORM_VALUE_ARRAY (GLint,   glUniform1iv)
    AA_GLSL_UNIFORM_VALUE_ARRAY (GLuint,  glUniform1uiv)
    AA_GLSL_UNIFORM_VALUE_ARRAY (GLfloat, glUniform1fv)

#undef AA_GLSL_UNIFORM_VALUE_ARRAY

#define AA_GLSL_UNIFORM_VECTOR_ARRAY(T, m, F) \
    template <>\
    inline\
    void SetUniform<T, m> (GLint location, GLsizei count, const V<T, m> * values)\
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

#define AA_GLSL_UNIFORM_MATRIX_ARRAY(T, m, n, F) \
    template <>\
    inline\
    void SetUniform<T, m, n> (GLint location, GLsizei count, const M<T, m, n> * values)\
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

  } // namespace GLSL
} // namespace Aa

#endif // AA_GLSL__H

