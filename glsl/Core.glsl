#ifndef AA_CORE
#define AA_CORE

uniform mat4 aa_gl_modelview;
uniform mat4 aa_gl_modelview_inverse;
uniform mat4 aa_gl_projection;
uniform mat4 aa_gl_projection_inverse;
uniform mat3 aa_gl_normal_matrix;
uniform vec2 aa_gl_depth_range;

float aa_gl_depth (vec4 p)
{
  float n = aa_gl_depth_range [0];
  float f = aa_gl_depth_range [1];
  return clamp ((-p.z - n) / (f - n), 0, 1);
}

#endif // AA_CORE
