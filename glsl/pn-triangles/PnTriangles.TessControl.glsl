#version 420 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/PnTriangles"
 
// tessellation levels
uniform float aa_pn_triangles_levels;
 
layout (vertices = 3) out;
 
in layout (location = 0) vec3 aa_vertex_normal [];
in layout (location = 1) vec4 aa_vertex_color  [];
 
out layout (location = 0) vec3    aa_control_normal [3];
out layout (location = 3) vec4    aa_control_color  [3];
out layout (location = 6) PnPatch aa_control_patch  [3];
 
float wij (int i, int j)
{
 return dot (gl_in[j].gl_Position.xyz - gl_in[i].gl_Position.xyz, aa_vertex_normal[i]);
}
 
float vij (int i, int j)
{
 vec3 Pj_minus_Pi = gl_in[j].gl_Position.xyz
                  - gl_in[i].gl_Position.xyz;
 vec3 Ni_plus_Nj  = aa_vertex_normal[i]+aa_vertex_normal[j];
 return 2.0 * dot (Pj_minus_Pi, Ni_plus_Nj) / dot (Pj_minus_Pi, Pj_minus_Pi);
}
 
void main ()
{
 // get data
 gl_out  [gl_InvocationID].gl_Position = gl_in   [gl_InvocationID].gl_Position;
 aa_control_normal [gl_InvocationID]             = aa_vertex_normal [gl_InvocationID];
 aa_control_color  [gl_InvocationID]             = aa_vertex_color  [gl_InvocationID];
 
 // set base 
 float P0 = gl_in[0].gl_Position[gl_InvocationID];
 float P1 = gl_in[1].gl_Position[gl_InvocationID];
 float P2 = gl_in[2].gl_Position[gl_InvocationID];
 float N0 = aa_vertex_normal[0][gl_InvocationID];
 float N1 = aa_vertex_normal[1][gl_InvocationID];
 float N2 = aa_vertex_normal[2][gl_InvocationID];
 
 // compute control points
 aa_control_patch[gl_InvocationID].b210 = (2.0*P0 + P1 - wij(0,1)*N0)/3.0;
 aa_control_patch[gl_InvocationID].b120 = (2.0*P1 + P0 - wij(1,0)*N1)/3.0;
 aa_control_patch[gl_InvocationID].b021 = (2.0*P1 + P2 - wij(1,2)*N1)/3.0;
 aa_control_patch[gl_InvocationID].b012 = (2.0*P2 + P1 - wij(2,1)*N2)/3.0;
 aa_control_patch[gl_InvocationID].b102 = (2.0*P2 + P0 - wij(2,0)*N2)/3.0;
 aa_control_patch[gl_InvocationID].b201 = (2.0*P0 + P2 - wij(0,2)*N0)/3.0;

 float E = (aa_control_patch[gl_InvocationID].b210
          + aa_control_patch[gl_InvocationID].b120
          + aa_control_patch[gl_InvocationID].b021
          + aa_control_patch[gl_InvocationID].b012
          + aa_control_patch[gl_InvocationID].b102
          + aa_control_patch[gl_InvocationID].b201) / 6.0;

 float V = (P0 + P1 + P2)/3.0;
 aa_control_patch[gl_InvocationID].b111 = E + (E - V)*0.5;
 aa_control_patch[gl_InvocationID].n110 = N0+N1-vij(0,1)*(P1-P0);
 aa_control_patch[gl_InvocationID].n011 = N1+N2-vij(1,2)*(P2-P1);
 aa_control_patch[gl_InvocationID].n101 = N2+N0-vij(2,0)*(P0-P2);
 
 // set tess levels
 gl_TessLevelOuter [gl_InvocationID] = aa_pn_triangles_levels;
 gl_TessLevelInner [0]               = aa_pn_triangles_levels;
}
