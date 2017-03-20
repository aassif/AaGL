#version 330 core

uniform mat4 aa_gl_modelview;
uniform mat4 aa_gl_projection;

layout (location = 0) in vec4 aa_gl_position;

out vec3 aa_vertex_position;

void main ()
{
  aa_vertex_position =                                      aa_gl_position.xyz;
  gl_Position        = aa_gl_projection * aa_gl_modelview * aa_gl_position;
}

