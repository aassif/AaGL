#ifndef AA_GL_FIXED_FUNCTION__H
#define AA_GL_FIXED_FUNCTION__H

namespace Aa
{
  namespace GL
  {

    class FixedFunction
    {
      public:
        inline static
        mat4 ModelView ()
        {
          mat4 m;
          glGetFloatv (GL_MODELVIEW_MATRIX, &(m[0][0]));
          return m;
        }

        inline static
        mat4 Projection ()
        {
          mat4 m;
          glGetFloatv (GL_PROJECTION_MATRIX, &(m[0][0]));
          return m;
        }

        inline static
        CoreContext Context (const vec2 & depth_range,
                             bool         motion = false)
        {
          return CoreContext (NULL,
                              ModelView (),
                              Projection (),
                              depth_range,
                              motion);
        }
    };

  } // namespace GL
} //namespace Aa

#endif // AA_GL_FIXED_FUNCTION__H

