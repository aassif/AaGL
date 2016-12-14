#ifndef AA_FRAGMENT
#define AA_FRAGMENT

#extension GL_ARB_shading_language_include : require

#include "/Aa/Phong"

in vec3 aa_vertex_normal;
in vec4 aa_vertex_color;

layout (location = 0) out vec4 aa_fragment_color;

void aa_gl_core ()
{
  aa_fragment_color = aa_phong (aa_vertex_normal, aa_vertex_color, 1);
}

#endif // AA_FRAGMENT

