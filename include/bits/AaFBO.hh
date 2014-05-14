#ifndef AA_GL_FBO__H
#define AA_GL_FBO__H

namespace Aa
{
  namespace GL
  {

    class FBO
    {
      public:
        enum
        {
          READ = 1,
          DRAW = 2,
          BOTH = READ | DRAW
        };

        enum
        {
          BLIT_COLOR   = GL_COLOR_BUFFER_BIT,
          BLIT_DEPTH   = GL_DEPTH_BUFFER_BIT,
          BLIT_STENCIL = GL_STENCIL_BUFFER_BIT,
          BLIT_ALL     = BLIT_COLOR | BLIT_DEPTH | BLIT_STENCIL
        };

      private:
        inline static
        GLenum TargetToken (int mode)
        {
          switch (mode)
          {
            case READ: return GL_READ_FRAMEBUFFER;
            case DRAW: return GL_DRAW_FRAMEBUFFER;
            case BOTH: return GL_FRAMEBUFFER;
            default:   return 0;
          }
        }

        inline static
        GLenum BindingToken (int mode)
        {
          switch (mode)
          {
            case READ: return GL_READ_FRAMEBUFFER_BINDING;
            case DRAW: return GL_DRAW_FRAMEBUFFER_BINDING;
            default:   return 0;
          }
        }

      public:
        inline static
        GLint Binding (int mode)
        {
          GLint framebuffer = 0;
          glGetIntegerv (BindingToken (mode), &framebuffer);
          return framebuffer;
        }

        inline static
        void Blit (GLuint src, const ibox2 & src_box, GLenum src_buffer,
                   GLuint dst, const ibox2 & dst_box, GLenum dst_buffer,
                   GLbitfield mask = FBO::BLIT_ALL, GLenum filter = GL_NEAREST)
        {
          //std::cout << "src = " << src << " : " << src_box << std::endl;
          ivec2 src_min = src_box.min ();
          ivec2 src_max = src_box.max ();

          //std::cout << "dst = " << dst << " : " << dst_box << std::endl;
          ivec2 dst_min = dst_box.min ();
          ivec2 dst_max = dst_box.max ();

          GLint read = FBO::Binding (FBO::READ);
          glBindFramebuffer (GL_READ_FRAMEBUFFER, src);
          glReadBuffer (src_buffer);

          GLint draw = FBO::Binding (FBO::DRAW);
          glBindFramebuffer (GL_DRAW_FRAMEBUFFER, dst);
          glDrawBuffer (dst_buffer);

          glBlitFramebuffer (src_min[0], src_min[1], src_max[0], src_max[1],
                             dst_min[0], dst_min[1], dst_max[0], dst_max[1],
                             mask, filter);

          glBindFramebuffer (GL_READ_FRAMEBUFFER, read);
          glBindFramebuffer (GL_DRAW_FRAMEBUFFER, draw);
        }

        inline static
        void Blit (const FBO * fbo,
                   const ibox2 & box,
                   GLenum buffer,
                   GLbitfield mask = FBO::BLIT_ALL,
                   GLenum filter = GL_NEAREST)
        {
          if (fbo != NULL)
          {
            GLint v [4];
            glGetIntegerv (GL_VIEWPORT, v);
            ibox2 dst (ivec2 (v[0], v[1]), ivec2 (v[2], v[3]));
            FBO::Blit (fbo->id (), box, buffer, 0, dst, GL_COLOR_ATTACHMENT0, mask, filter);
          }
        }

        inline static
        void Blit (const FBO * fbo,
                   GLenum buffer = GL_COLOR_ATTACHMENT0,
                   GLbitfield mask = FBO::BLIT_ALL,
                   GLenum filter = GL_NEAREST)
        {
          if (fbo != NULL)
            FBO::Blit (fbo, ibox2 (ivec2 (0), fbo->dims ()), buffer, mask, filter);
        }

        inline static
        void CheckStatus (int mode)
          //throw (FramebufferException)
        {
          GLenum status = glCheckFramebufferStatus (FBO::TargetToken (mode));
          switch (status)
          {
            case GL_FRAMEBUFFER_COMPLETE:                      break;
            case GL_FRAMEBUFFER_UNDEFINED:                     std::cerr << "GL_FRAMEBUFFER_UNDEFINED"                     << std::endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:         std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"         << std::endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"        << std::endl; break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:        std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"        << std::endl; break;
            case GL_FRAMEBUFFER_UNSUPPORTED:                   std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED"                   << std::endl; break;
            default:                                           std::cerr << status                                         << std::endl; break;
          }
        }

      private:
        uvec2               m_dims;
        GLuint              m_id;
        std::vector<GLuint> m_colors;
        GLuint              m_depth;
        GLuint              m_stencil;
        GLuint              m_depth_stencil;

      private:
        inline
        void init_depth ()
        {
          glGenTextures (1, &m_depth);
          glBindTexture (GL_TEXTURE_2D, m_depth);

          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

          if (GLEW_ARB_texture_storage)
            glTexStorage2D (GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, m_dims[0], m_dims[1]);
          else
            glTexImage2D (GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_dims[0], m_dims[1], 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

          glFramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depth, 0);
        }

        inline
        void init_stencil ()
        {
          glGenTextures (1, &m_stencil);
          glBindTexture (GL_TEXTURE_2D, m_stencil);

          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

          if (GLEW_ARB_texture_storage)
            glTexStorage2D (GL_TEXTURE_2D, 1, GL_STENCIL_INDEX8, m_dims[0], m_dims[1]);
          else
            glTexImage2D (GL_TEXTURE_2D, 0, GL_STENCIL_INDEX8, m_dims[0], m_dims[1], 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, NULL);

          glFramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencil, 0);
        }

        inline
        void init_depth_stencil ()
        {
          glGenTextures (1, &m_depth_stencil);
          glBindTexture (GL_TEXTURE_2D, m_depth_stencil);

#if 0
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#endif

          glTexStorage2D (GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_dims[0], m_dims[1]);
          glFramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,   GL_TEXTURE_2D, m_depth_stencil, 0);
          glFramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depth_stencil, 0);

          glGenTextures (1, &m_depth);
          glTextureView (m_depth, GL_TEXTURE_2D, m_depth_stencil, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);
          glBindTexture (GL_TEXTURE_2D, m_depth);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

          glGenTextures (1, &m_stencil);
          glTextureView (m_stencil, GL_TEXTURE_2D, m_depth_stencil, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);
          glBindTexture (GL_TEXTURE_2D, m_stencil);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
          glTexParameteri (GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
        }

        inline
        void init_colors (const std::vector<GLenum> & formats)
        {
          AaUInt n = formats.size ();
          m_colors = std::vector<GLuint> (n);
          glGenTextures (n, &(m_colors[0]));

          for (AaUInt i = 0; i < n; ++i)
          {
            glBindTexture (GL_TEXTURE_2D, m_colors [i]);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            if (GLEW_ARB_texture_storage)
              glTexStorage2D (GL_TEXTURE_2D, 1, formats [i], m_dims [0], m_dims [1]);
            else
              glTexImage2D (GL_TEXTURE_2D, 0, formats [i], m_dims[0], m_dims[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glFramebufferTexture2D (GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colors [i], 0);
          }
        }

        inline
        void init (const std::vector<GLenum> & formats, bool depth, bool stencil)
          //throw (FramebufferException)
        {
          GLint draw = FBO::Binding (DRAW);

          glGenFramebuffers (1, &m_id);
          glBindFramebuffer (GL_DRAW_FRAMEBUFFER, m_id);

          this->init_colors (formats);

          if (depth)
          {
            if (stencil) this->init_depth_stencil ();
            else         this->init_depth         ();
          }
          else
            if (stencil) this->init_stencil       ();

          FBO::CheckStatus (FBO::DRAW);
          glBindFramebuffer (GL_DRAW_FRAMEBUFFER, draw);
        }

      public:
        inline explicit
        FBO (const uvec2 & d,
             const std::vector<GLenum> formats,
             bool depth   = true,
             bool stencil = false) :
          m_dims (d),
          m_id (0),
          m_colors (),
          m_depth (0),
          m_stencil (0),
          m_depth_stencil (0)
        {
          this->init (formats, depth, stencil);
        }

        inline explicit
        FBO (const uvec2 & d,
             GLenum color0,
             bool depth   = true,
             bool stencil = false) :
          m_dims (d),
          m_id (0),
          m_colors (),
          m_depth (0),
          m_stencil (0),
          m_depth_stencil (0)
        {
          std::vector<GLenum> formats;
          formats.push_back (color0);
          this->init (formats, depth, stencil);
        }

        inline explicit
        FBO (const uvec2 & d,
             GLenum color0,
             GLenum color1,
             bool depth   = true,
             bool stencil = false) :
          m_dims (d),
          m_id (0),
          m_colors (),
          m_depth (0),
          m_stencil (0),
          m_depth_stencil (0)
        {
          std::vector<GLenum> formats;
          formats.push_back (color0);
          formats.push_back (color1);
          this->init (formats, depth, stencil);
        }

        inline
        ~FBO ()
        {
          glDeleteFramebuffers (1, &m_id);
          glDeleteTextures (1, &m_depth_stencil);
          glDeleteTextures (1, &m_depth);
          glDeleteTextures (1, &m_stencil);
          for (AaUInt i = 0; i < m_colors.size (); ++i)
            glDeleteTextures (1, &(m_colors[i]));
        }

        inline const uvec2 & dims () const {return m_dims;}

        inline GLuint id      () const {return m_id;}

        inline GLuint depth   () const {return m_depth;}
        inline GLuint stencil () const {return m_stencil;}

        inline GLuint texture (AaUInt k) const {return m_colors [k];}

        inline
        void bind (int mode)
        {
          glBindFramebuffer (FBO::TargetToken (mode), m_id);
          if (mode & DRAW) this->set_draw_buffers ();
        }

        inline
        void clear_depth (GLfloat d = 1.0f)
        {
          glClearBufferfv (GL_DEPTH, 0, &d);
        }

        inline
        void clear_stencil (GLint s = 0)
        {
          glClearBufferiv (GL_STENCIL, 0, &s);
        }

        inline
        void clear_color (AaUInt k, const vec4 & c = vec4 (0.0f))
        {
          glClearBufferfv (GL_COLOR, k, &(c[0]));
        }

        inline
        void set_draw_buffers () const
        {
          AaUInt n = m_colors.size ();
          std::vector<GLenum> buffers (n);
          for (AaUInt i = 0; i < n; ++i)
            buffers [i] = GL_COLOR_ATTACHMENT0 + i;
          glDrawBuffers (n, &(buffers[0]));
        }
    };

  } // namespace GL
} // namespace Aa

#endif // AA_GL_FBO__H

