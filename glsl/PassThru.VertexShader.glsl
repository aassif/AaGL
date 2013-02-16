#version 330 core

layout (location = 0) in vec4 aa_gl_position;

void main ()
{
  gl_Position = aa_gl_position;
}

