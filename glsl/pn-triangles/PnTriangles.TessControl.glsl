#version 420 core

#extension GL_ARB_shading_language_include : require

uniform float aa_pn_triangles_levels;
 
in layout (location = 0) vec3 aa_vertex_normal [];
in layout (location = 1) vec4 aa_vertex_color  [];
 
layout (vertices = 3) out;

out vec3    aa_control_normal [3];
out vec4    aa_control_color  [3];
 
void main ()
{
  gl_out            [gl_InvocationID].gl_Position = gl_in            [gl_InvocationID].gl_Position;
  aa_control_normal [gl_InvocationID]             = aa_vertex_normal [gl_InvocationID];
  aa_control_color  [gl_InvocationID]             = aa_vertex_color  [gl_InvocationID];
 
  gl_TessLevelOuter [0] = aa_pn_triangles_levels;
  gl_TessLevelOuter [1] = aa_pn_triangles_levels;
  gl_TessLevelOuter [2] = aa_pn_triangles_levels;
  gl_TessLevelInner [0] = aa_pn_triangles_levels;
}

