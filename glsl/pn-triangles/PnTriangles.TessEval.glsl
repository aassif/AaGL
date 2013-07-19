#version 420 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"

uniform float aa_pn_triangles_weight;

layout (triangles, equal_spacing, ccw) in;

in vec3 aa_control_normal [];
in vec4 aa_control_color  [];
in vec2 aa_control_tex2d  [];

out vec4  aa_eval_position;
out vec3  aa_eval_normal;
out vec4  aa_eval_color;
out vec2  aa_eval_tex2d;
out float aa_eval_depth;

void main ()
{
  vec3 p1 = gl_in[0].gl_Position.xyz;
  vec3 p2 = gl_in[1].gl_Position.xyz;
  vec3 p3 = gl_in[2].gl_Position.xyz;

  vec3 n1 = aa_control_normal[0];
  vec3 n2 = aa_control_normal[1];
  vec3 n3 = aa_control_normal[2];

  float u = gl_TessCoord.x;
  float v = gl_TessCoord.y;
  float w = gl_TessCoord.z;

  vec3 b300 = p1;
  vec3 b030 = p2;
  vec3 b003 = p3;

  float w12 = dot (p2 - p1, n1);
  float w21 = dot (p1 - p2, n2);
  float w13 = dot (p3 - p1, n1);
  float w31 = dot (p1 - p3, n3);
  float w23 = dot (p3 - p2, n2);
  float w32 = dot (p2 - p3, n3);

  vec3 b210 = (2.0 * p1 + p2 - w12 * n1) / 3.0;
  vec3 b120 = (2.0 * p2 + p1 - w21 * n2) / 3.0;
  vec3 b021 = (2.0 * p2 + p3 - w23 * n2) / 3.0;
  vec3 b012 = (2.0 * p3 + p2 - w32 * n3) / 3.0;
  vec3 b102 = (2.0 * p3 + p1 - w31 * n3) / 3.0;
  vec3 b201 = (2.0 * p1 + p3 - w13 * n1) / 3.0;

  vec3 b111a = (b210 + b120 + b021 + b012 + b102 + b201) / 6.0;
  vec3 b111b = (p1 + p2 + p3) / 3.0;
  vec3 b111 = mix (b111a, b111b, aa_pn_triangles_weight);

  vec3 p = 1.0 * b300 * w * w * w
         + 1.0 * b030 * u * u * u
         + 1.0 * b003 * v * v * v
         + 3.0 * b210 * u * w * w
         + 3.0 * b120 * u * u * w
         + 3.0 * b201 * v * w * w
         + 3.0 * b021 * u * u * v
         + 3.0 * b102 * v * v * w
         + 3.0 * b012 * u * v * v
         + 6.0 * b111 * u * v * w;

  float v12 = 2.0 * dot (p2 - p1, n1 + n2) / dot (p2 - p1, p2 - p1);
  float v23 = 2.0 * dot (p3 - p2, n2 + n3) / dot (p3 - p2, p3 - p2);
  float v31 = 2.0 * dot (p1 - p3, n3 + n1) / dot (p1 - p3, p1 - p3);

  vec3 n200 = n1;
  vec3 n020 = n2;
  vec3 n002 = n3;
  vec3 n110 = normalize (n1 + n2 - v12 * (p2 - p1));
  vec3 n011 = normalize (n2 + n3 - v23 * (p3 - p2));
  vec3 n101 = normalize (n3 + n1 - v31 * (p1 - p3));

  vec4 color = w * aa_control_color[0]
             + u * aa_control_color[1]
             + v * aa_control_color[2];

  vec3 normal = n200 * w * w
              + n020 * u * u
              + n002 * v * v
              + n110 * w * u
              + n011 * u * v
              + n101 * w * v;

  aa_eval_position = vec4 (p, 1.0);
  aa_eval_normal   = normal;
  aa_eval_color    = color;
  aa_eval_depth    = aa_gl_depth       (aa_eval_position);
  gl_Position      = aa_gl_projection * aa_eval_position;
}

