#version 420 core
 
#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"
#include "/Aa/PhongTess"

uniform float aa_phong_tess_weight;
 
layout (triangles, fractional_odd_spacing, ccw) in;
 
in layout (location = 0) vec3       aa_control_normal [];
in layout (location = 0) vec4       aa_control_color  [];
in layout (location = 6) PhongPatch aa_control_patch  [];
 
out vec4  aa_eval_position;
out vec3  aa_eval_normal;
out vec4  aa_eval_color;
out float aa_eval_depth;

#define Pi  gl_in[0].gl_Position.xyz
#define Pj  gl_in[1].gl_Position.xyz
#define Pk  gl_in[2].gl_Position.xyz
#define tc1 gl_TessCoord
 
void main ()
{
  // precompute squared tesscoords
  vec3 tc2 = tc1*tc1;
    
  // compute texcoord and normal
//  oTexCoord = gl_TessCoord[0]*iTexCoord[0]
//            + gl_TessCoord[1]*iTexCoord[1]
//            + gl_TessCoord[2]*iTexCoord[2];
  vec3 normal   = gl_TessCoord[0] * aa_control_normal[0]
                + gl_TessCoord[1] * aa_control_normal[1]
                + gl_TessCoord[2] * aa_control_normal[2];
      
  // interpolated position
  vec3 p1 = gl_TessCoord[0] * Pi
          + gl_TessCoord[1] * Pj
          + gl_TessCoord[2] * Pk;
       
  // build terms
  vec3 termIJ = vec3 (aa_control_patch[0].termIJ,
                      aa_control_patch[1].termIJ,
                      aa_control_patch[2].termIJ);
  vec3 termJK = vec3 (aa_control_patch[0].termJK,
                      aa_control_patch[1].termJK,
                      aa_control_patch[2].termJK);
  vec3 termIK = vec3 (aa_control_patch[0].termIK,
                      aa_control_patch[1].termIK,
                      aa_control_patch[2].termIK);

  // phong tesselated pos
  vec3 p2 = tc2[0] * Pi
          + tc2[1] * Pj
          + tc2[2] * Pk
          + tc1[0] * tc1[1] * termIJ
          + tc1[1] * tc1[2] * termJK
          + tc1[2] * tc1[0] * termIK;
           
  // final position
  vec3 position = mix (p1, p2, aa_phong_tess_weight);

  aa_eval_position = aa_gl_modelview     * vec4 (position, 1.0);
  aa_eval_normal   = aa_gl_normal_matrix * normal;
  aa_eval_color    = vec4 (1, 0, 0, 1); //aa_gl_color;
  aa_eval_depth    = aa_gl_depth          (aa_eval_position);
  gl_Position      = aa_gl_projection    * aa_eval_position;
}

