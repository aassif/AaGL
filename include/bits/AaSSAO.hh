#ifndef AA_SSAO__H
#define AA_SSAO__H

#include <AaMeshVAO>
#include <cstdlib>

namespace Aa
{
  namespace GL
  {

    class Quad
    {
      private:
        GLuint m_vbo;
        GLuint m_vao;

      public:
        Quad () :
          m_vbo (),
          m_vao ()
        {
          static const GLfloat QUAD [4][2] = {{-1, -1}, {-1, +1}, {+1, -1}, {+1, +1}};

          glGenBuffers (1, &m_vbo);
          glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
          glBufferData (GL_ARRAY_BUFFER, 8 * sizeof (GLfloat), QUAD, GL_STATIC_DRAW);
          glBindBuffer (GL_ARRAY_BUFFER, 0);

          glGenVertexArrays (1, &m_vao);
          glBindVertexArray (m_vao);
          glBindBuffer (GL_ARRAY_BUFFER, m_vbo);
          glEnableVertexAttribArray (0);
          glVertexAttribPointer (0, 2, GL_FLOAT, GL_FALSE, 0, 0);
          glBindVertexArray (0);
        }

        void draw () const
        {
          GLboolean culling = glIsEnabled (GL_CULL_FACE);
          glDisable (GL_CULL_FACE);
          glBindVertexArray (m_vao);
          glDrawArrays (GL_TRIANGLE_STRIP, 0, 4);
          glBindVertexArray (0);
          if (culling) glEnable (GL_CULL_FACE);
        }
    };

    class SSAO
    {
      private:
        GLuint m_random;
        uvec2  m_random_size;
        FBO  * m_fbo;
        uvec2  m_fbo_size;
        Quad   m_quad;

        GLSL::Program          m_program;
        GLSL::Location<mat4>   m_program_projection;
        GLSL::Location<vec2>   m_program_depth_range;
        GLSL::Location<int>    m_program_randomized;
        GLSL::Location<uvec2>  m_program_screen_size;
        GLSL::Location<uvec2>  m_program_random_size;
        GLSL::Location<float>  m_program_radius;
        GLSL::Location<AaUInt> m_program_rings;
        GLSL::Location<AaUInt> m_program_slices;
        GLSL::Location<int>    m_program_tex2d_color;
        GLSL::Location<int>    m_program_tex2d_depth;
        GLSL::Location<int>    m_program_tex2d_random;

      public:
        SSAO (const std::string & vertex   = "/Aa/PassThru.vertex",
              const std::string & fragment = "/Aa/SSAO.fragment") :
          m_random (0),
          m_random_size (64u),
          m_fbo (NULL),
          m_fbo_size (0u),
          m_quad (),
          m_program ()
        {
          GLubyte R [64][64];
          for (AaUInt i = 0; i < 64; ++i)
            for (AaUInt j = 0; j < 64; ++j)
              R [i][j] = rand () % 256;

          glGenTextures (1, &m_random);
          glBindTexture (GL_TEXTURE_2D, m_random);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
          glTexImage2D (GL_TEXTURE_2D, 0, GL_R8, 64, 64, 0, GL_RED, GL_UNSIGNED_BYTE, R);
          glGenerateMipmap (GL_TEXTURE_2D);
          glBindTexture (GL_TEXTURE_2D, 0);

          m_program.attach (GL_VERTEX_SHADER,   vertex);
          m_program.attach (GL_FRAGMENT_SHADER, fragment);
          m_program.link ();

          m_program_projection   = m_program.location ("aa_ssao_projection");
          m_program_depth_range  = m_program.location ("aa_ssao_depth_range");
          m_program_randomized   = m_program.location ("aa_ssao_randomized");
          m_program_screen_size  = m_program.location ("aa_ssao_screen_size");
          m_program_random_size  = m_program.location ("aa_ssao_random_size");
          m_program_radius       = m_program.location ("aa_ssao_radius");
          m_program_rings        = m_program.location ("aa_ssao_rings");
          m_program_slices       = m_program.location ("aa_ssao_slices");
          m_program_tex2d_color  = m_program.location ("aa_ssao_tex2d_color");
          m_program_tex2d_depth  = m_program.location ("aa_ssao_tex2d_depth");
          m_program_tex2d_random = m_program.location ("aa_ssao_tex2d_random");
        }

        void bind (const Aa::uvec2 & d)
        {
          if (d != m_fbo_size)
          {
            if (m_fbo != NULL)
            {
              delete m_fbo;
              m_fbo = NULL;
              m_fbo_size = uvec2 (0u);
            }

            m_fbo = new FBO (d, GL_RGBA8, GL_R32F, true, false);
            m_fbo_size = d;
          }

          m_fbo->bind (FBO::DRAW);
          m_fbo->clear_depth (1.0f);
          m_fbo->clear_color (0, vec4 (0.0f));
          m_fbo->clear_color (1, vec4 (1.0f));
        }

        void draw (const CoreContext & context, float radius)
        {
          //Aa::GL::FBO::Blit (m_fbo, GL_COLOR_ATTACHMENT0, Aa::GL::FBO::BLIT_COLOR); return;

          glActiveTexture (GL_TEXTURE0); glBindTexture (GL_TEXTURE_2D, m_fbo->texture (0));
          glActiveTexture (GL_TEXTURE1); glBindTexture (GL_TEXTURE_2D, m_fbo->texture (1));
          glActiveTexture (GL_TEXTURE2); glBindTexture (GL_TEXTURE_2D, m_random);

          m_program.use ();

          m_program_projection   (context.projection ());
          m_program_depth_range  (context.depth_range ());
          m_program_randomized   (0);
          m_program_screen_size  (m_fbo_size);
          m_program_random_size  (m_random_size);
          m_program_radius       (radius);
          m_program_rings        (8u);
          m_program_slices       (16u);
          m_program_tex2d_color  (0);
          m_program_tex2d_depth  (1);
          m_program_tex2d_random (2);

          m_quad.draw ();

          glUseProgram (0);

          glActiveTexture (GL_TEXTURE0); glBindTexture (GL_TEXTURE_2D, 0);
          glActiveTexture (GL_TEXTURE1); glBindTexture (GL_TEXTURE_2D, 0);
          glActiveTexture (GL_TEXTURE2); glBindTexture (GL_TEXTURE_2D, 0);
        }
    };

  } //namespace GL
} // namespace Aa

#endif // AA_SSAO__H

