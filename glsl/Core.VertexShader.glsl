#version 330 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"
#include "/Aa/Core/VertexIn"
#include "/Aa/Core/VertexOut"

void main ()
{
  aa_vertex_position = aa_gl_modelview     * aa_gl_position;
  aa_vertex_normal   = aa_gl_normal_matrix * aa_gl_normal;
  aa_vertex_color    = aa_gl_color;

  gl_Position        = aa_gl_projection    * aa_vertex_position;
}

