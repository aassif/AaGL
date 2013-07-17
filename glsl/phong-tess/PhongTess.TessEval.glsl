#version 420 core
 
#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"
#include "/Aa/PhongTess"

uniform float aa_phong_tess_weight;
 
layout (triangles, equal_spacing, ccw) in;
 
in vec3       aa_control_normal [];
in vec4       aa_control_color  [];
in vec2       aa_control_tex2d  [];
in PhongPatch aa_control_patch  [];
 
out vec4  aa_eval_position;
out vec3  aa_eval_normal;
out vec4  aa_eval_color;
out vec2  aa_eval_tex2d;
out float aa_eval_depth;

#define Pi  gl_in[0].gl_Position.xyz
#define Pj  gl_in[1].gl_Position.xyz
#define Pk  gl_in[2].gl_Position.xyz
#define uvw gl_TessCoord
 
void main ()
{
  float u = uvw [0], u2 = u * u;
  float v = uvw [1], v2 = v * v;
  float w = uvw [2], w2 = w * w;
    
  vec3 normal  = u * aa_control_normal[0]
               + v * aa_control_normal[1]
               + w * aa_control_normal[2];
      
  vec3 color   = u * aa_control_color[0]
               + v * aa_control_color[1]
               + w * aa_control_color[2];
      
//  vec2 tex2d   = u * aa_control_tex2d[0]
//               + v * aa_control_tex2d[1]
//               + w * aa_control_tex2d[2];

  vec3 p1 = u * Pi + v * Pj + w * Pk;
       
  vec3 termIJ = vec3 (aa_control_patch[0].termIJ,
                      aa_control_patch[1].termIJ,
                      aa_control_patch[2].termIJ);
  vec3 termJK = vec3 (aa_control_patch[0].termJK,
                      aa_control_patch[1].termJK,
                      aa_control_patch[2].termJK);
  vec3 termIK = vec3 (aa_control_patch[0].termIK,
                      aa_control_patch[1].termIK,
                      aa_control_patch[2].termIK);

  vec3 p2 = u2 * Pi
          + v2 * Pj
          + w2 * Pk
          + u * v * termIJ
          + v * w * termJK
          + w * u * termIK;
           
  vec3 position = mix (p1, p2, aa_phong_tess_weight);

  aa_eval_position = vec4 (position, 1.0);
  aa_eval_normal   = normal;
  aa_eval_color    = color;
  aa_eval_depth    = aa_gl_depth       (aa_eval_position);
  gl_Position      = aa_gl_projection * aa_eval_position;
}

