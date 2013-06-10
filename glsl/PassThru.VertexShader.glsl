#version 330 core

in layout (location = 0) vec4 aa_gl_position;

out vec4 aa_pass_thru_position;

void main ()
{
  gl_Position           = aa_gl_position;
  aa_pass_thru_position = aa_gl_position;
}

