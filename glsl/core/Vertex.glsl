#ifndef AA_VERTEX
#define AA_VERTEX

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"

layout (location = 0) in vec4 aa_gl_position;
layout (location = 1) in vec3 aa_gl_normal;
layout (location = 2) in vec4 aa_gl_color;

out vec4  aa_vertex_position;
out vec3  aa_vertex_normal;
out vec4  aa_vertex_color;
out float aa_vertex_depth;

void aa_gl_core ()
{
  aa_vertex_position = aa_gl_modelview     * aa_gl_position;
  aa_vertex_normal   = aa_gl_normal_matrix * aa_gl_normal;
  aa_vertex_color    = aa_gl_color;
  aa_vertex_depth    = aa_gl_depth          (aa_vertex_position);
  gl_Position        = aa_gl_projection    * aa_vertex_position;
}

#endif // AA_VERTEX

