#version 330 core

uniform vec4 aa_gl_fill_color;

layout (location = 0) out vec4 aa_gl_fragment_color;

void main ()
{
  aa_gl_fragment_color = aa_gl_fill_color;
}

