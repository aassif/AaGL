#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/SSAO"

layout (location = 0) out vec4 aa_fragment_color;

void main ()
{
  vec2 p = gl_FragCoord.xy / aa_ssao_screen_size;
  vec4 c = texture (aa_ssao_tex2d_color, p);
  float z = texture (aa_ssao_tex2d_depth, p).r;
  if (z == 1.0) discard;
  float o = aa_ssao_occlusion (p, z);
  aa_fragment_color = c * o;
  gl_FragDepth = aa_ssao_project (z);
}
  
