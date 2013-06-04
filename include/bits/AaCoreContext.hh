#ifndef AA_GL_CORE_CONTEXT__H
#define AA_GL_CORE_CONTEXT__H

namespace Aa
{
  namespace GL
  {

    class CoreContext
    {
      public:
        inline static
        mat3 NormalMatrix (const mat4 & modelview)
        {
          return mat3 (modelview).inv ().transpose ();
        }

      private:
        mat4 m_modelview;
        mat4 m_projection;
        vec2 m_depth_range;
        bool m_moving;

      public:
        inline
        CoreContext (const mat4 & modelview,
                     const mat4 & projection,
                     const vec2 & depth_range,
                     bool         moving = false) :
          m_modelview   (modelview),
          m_projection  (projection),
          m_depth_range (depth_range),
          m_moving      (moving)
        {
        }

        inline const mat4 & modelview   () const {return m_modelview;}
        inline const mat4 & projection  () const {return m_projection;}
        inline const vec2 & depth_range () const {return m_depth_range;}
        inline bool         is_moving   () const {return m_moving;}
    };

  } // namespace GL
} //namespace Aa

#endif // AA_GL_CORE_CONTEXT__H

