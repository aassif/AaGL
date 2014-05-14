#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/SSAO"

in vec4 aa_pass_thru_position;

out layout (location = 0) vec4 aa_fragment_color;

void main ()
{
  vec2  p = 0.5 + 0.5 * aa_pass_thru_position.xy;
  vec4  c = texture (aa_ssao_tex2d_color, p);
  float z = texture (aa_ssao_tex2d_depth, p).r;
  if (z == 1.0) discard;
  float o = aa_ssao_occlusion (p, z);
  aa_fragment_color = vec4 (c.rgb * o, c.a);
  gl_FragDepth = aa_ssao_project (z);
}
  
