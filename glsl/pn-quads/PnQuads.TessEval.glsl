#version 420 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"

uniform float aa_pn_quads_weight;

layout (quads, equal_spacing, ccw) in;

in vec3 aa_control_normal [];
in vec4 aa_control_color  [];
in vec2 aa_control_tex2d  [];

out vec4  aa_eval_position;
out vec3  aa_eval_normal;
out vec4  aa_eval_color;
out vec2  aa_eval_tex2d;
out float aa_eval_depth;

vec4 aa_pn_quads_basis (float x)
{
  return vec4 (1.0, x, x * x, x * x * x);
}

vec3 aa_pn_quads_boundary (vec3 p0, vec3 p1, vec3 n0)
{
  return (2.0 * p0 + p1 - dot (p1 - p0, n0) * n0) / 3.0;
}

vec3 aa_pn_quads_normal (vec3 p0, vec3 p1, vec3 n0, vec3 n1)
{
  return normalize (n0 + n1 - (2.0 * dot (p1 - p0, n0 + n1) / dot (p1 - p0, p1 - p0)) * (p1 - p0));
}

void main ()
{
  float u = gl_TessCoord.x;
  float v = gl_TessCoord.y;

  vec3 b00 = gl_in[0].gl_Position.xyz;
  vec3 b03 = gl_in[1].gl_Position.xyz;
  vec3 b30 = gl_in[2].gl_Position.xyz;
  vec3 b33 = gl_in[3].gl_Position.xyz;

  vec3 n00 = aa_control_normal[0];
  vec3 n02 = aa_control_normal[1];
  vec3 n20 = aa_control_normal[2];
  vec3 n22 = aa_control_normal[3];

  vec3 b01 = aa_pn_quads_boundary (b00, b03, n00);
  vec3 b02 = aa_pn_quads_boundary (b03, b00, n02);
  vec3 b10 = aa_pn_quads_boundary (b00, b30, n00);
  vec3 b13 = aa_pn_quads_boundary (b03, b33, n02);
  vec3 b20 = aa_pn_quads_boundary (b30, b00, n20);
  vec3 b23 = aa_pn_quads_boundary (b33, b03, n22);
  vec3 b31 = aa_pn_quads_boundary (b30, b33, n20);
  vec3 b32 = aa_pn_quads_boundary (b33, b30, n22);

  vec3 q = b01 + b02 + b10 + b13 + b20 + b23 + b31 + b32;

  vec3 b11 = mix (2.0 * (b01 + b10 + q) - (b23 + b32), 8.0 * b00 + 4.0 * (b03 + b30) + 2.0 * b33, aa_pn_quads_weight) / 18.0;
  vec3 b12 = mix (2.0 * (b02 + b13 + q) - (b20 + b31), 8.0 * b03 + 4.0 * (b00 + b33) + 2.0 * b30, aa_pn_quads_weight) / 18.0;
  vec3 b21 = mix (2.0 * (b20 + b31 + q) - (b02 + b13), 8.0 * b30 + 4.0 * (b00 + b33) + 2.0 * b03, aa_pn_quads_weight) / 18.0;
  vec3 b22 = mix (2.0 * (b23 + b32 + q) - (b01 + b10), 8.0 * b33 + 4.0 * (b03 + b30) + 2.0 * b00, aa_pn_quads_weight) / 18.0;

  mat4 Bx = mat4 (b00.x, b01.x, b02.x, b03.x, b10.x, b11.x, b12.x, b13.x, b20.x, b21.x, b22.x, b23.x, b30.x, b31.x, b32.x, b33.x);
  mat4 By = mat4 (b00.y, b01.y, b02.y, b03.y, b10.y, b11.y, b12.y, b13.y, b20.y, b21.y, b22.y, b23.y, b30.y, b31.y, b32.y, b33.y);
  mat4 Bz = mat4 (b00.z, b01.z, b02.z, b03.z, b10.z, b11.z, b12.z, b13.z, b20.z, b21.z, b22.z, b23.z, b30.z, b31.z, b32.z, b33.z);

  mat4 M4 = mat4 (+1, -3, +3, -1, 0, +3, -6, +3, 0, 0, +3, -3, 0, 0, 0, +1);
  mat4 M4t = transpose (M4);

  vec4 U4 = aa_pn_quads_basis (u);
  vec4 V4 = aa_pn_quads_basis (v);

  mat4 M4x = M4 * Bx * M4t;
  mat4 M4y = M4 * By * M4t;
  mat4 M4z = M4 * Bz * M4t;

  float px = dot (U4, M4x * V4);
  float py = dot (U4, M4y * V4);
  float pz = dot (U4, M4z * V4);

  vec4 position = vec4 (px, py, pz, 1.0);

  vec3 n01 = aa_pn_quads_normal (b00, b03, n00, n02);
  vec3 n10 = aa_pn_quads_normal (b00, b30, n00, n20);
  vec3 n12 = aa_pn_quads_normal (b03, b33, n02, n22);
  vec3 n21 = aa_pn_quads_normal (b30, b33, n20, n22);

  vec3 n11 = (2.0 * (n01 + n10 + n12 + n21) + 1.0 * (n00 + n02 + n20 + n22)) / 12.0;

  mat3 Nx = mat3 (n00.x, n01.x, n02.x, n10.x, n11.x, n12.x, n20.x, n21.x, n22.x);
  mat3 Ny = mat3 (n00.y, n01.y, n02.y, n10.y, n11.y, n12.y, n20.y, n21.y, n22.y);
  mat3 Nz = mat3 (n00.z, n01.z, n02.z, n10.z, n11.z, n12.z, n20.z, n21.z, n22.z);

  mat3 M3 = mat3 (+1, -2, +1, 0, +2, -2, 0, 0, +1);
  mat3 M3t = transpose (M3);

  vec3 U3 = U4.xyz;
  vec3 V3 = V4.xyz;

  mat3 M3x = M3 * Nx * M3t;
  mat3 M3y = M3 * Ny * M3t;
  mat3 M3z = M3 * Nz * M3t;

  float nx = dot (U3, M3x * V3);
  float ny = dot (U3, M3y * V3);
  float nz = dot (U3, M3z * V3);

  vec3 normal = normalize (vec3 (nx, ny, nz));

  vec4 color = mix (mix (aa_control_color[0], aa_control_color[1], u),
                    mix (aa_control_color[2], aa_control_color[3], u), v);

  aa_eval_position = position;
  aa_eval_normal   = normal;
  aa_eval_color    = color;
  aa_eval_tex2d    = gl_TessCoord.xy;
  aa_eval_depth    = aa_gl_depth       (aa_eval_position);
  gl_Position      = aa_gl_projection * aa_eval_position;
}

