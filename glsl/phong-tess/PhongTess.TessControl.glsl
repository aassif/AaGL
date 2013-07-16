#version 420 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/PhongTess"
 
uniform float aa_phong_tess_levels;
 
in layout (location = 0) vec3 aa_vertex_normal [];
in layout (location = 1) vec4 aa_vertex_color  [];
 
out layout (location = 0) vec3       aa_control_normal [3];
out layout (location = 3) vec4       aa_control_color  [3];
out layout (location = 6) PhongPatch aa_control_patch  [3];
 
layout (vertices = 3) out;

#define Pi gl_in[0].gl_Position.xyz
#define Pj gl_in[1].gl_Position.xyz
#define Pk gl_in[2].gl_Position.xyz
 
float aa_phong_tess_pi (int i, vec3 q)
{
  vec3 q_minus_p = q - gl_in[i].gl_Position.xyz;
  return q [gl_InvocationID] - dot(q_minus_p, aa_vertex_normal[i]) * aa_vertex_normal[i][gl_InvocationID];
}
 
void main ()
{
  // get data
  gl_out            [gl_InvocationID].gl_Position = gl_in            [gl_InvocationID].gl_Position;
  aa_control_normal [gl_InvocationID]             = aa_vertex_normal [gl_InvocationID];
      
  // compute patch data
  aa_control_patch [gl_InvocationID].termIJ = aa_phong_tess_pi (0, Pj) + aa_phong_tess_pi (1, Pi);
  aa_control_patch [gl_InvocationID].termJK = aa_phong_tess_pi (1, Pk) + aa_phong_tess_pi (2, Pj);
  aa_control_patch [gl_InvocationID].termIK = aa_phong_tess_pi (2, Pi) + aa_phong_tess_pi (0, Pk);
         
  // tesselate
  gl_TessLevelOuter [gl_InvocationID] = aa_phong_tess_levels;
  gl_TessLevelInner [0]               = aa_phong_tess_levels;
}

