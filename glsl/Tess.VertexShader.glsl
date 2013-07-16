#version 330 core

in layout (location = 0) vec4 aa_gl_position;
in layout (location = 1) vec3 aa_gl_normal;
in layout (location = 2) vec4 aa_gl_color;

out vec4 aa_vertex_position;
out vec3 aa_vertex_normal;
out vec4 aa_vertex_color;

void main ()
{
  gl_Position        = aa_gl_position;
  aa_vertex_position = aa_gl_position;
  aa_vertex_normal   = aa_gl_normal;
  aa_vertex_color    = aa_gl_color;
}

