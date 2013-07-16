#version 420 core

#extension GL_ARB_shading_language_include : require

#include "/Aa/Core"
#include "/Aa/PnTriangles"

uniform float aa_pn_triangles_weight;

layout (triangles, fractional_odd_spacing, ccw) in;

in layout (location = 0) vec3    aa_control_normal [];
in layout (location = 3) vec4    aa_control_color  [];
in layout (location = 6) PnPatch aa_control_patch  [];

out vec4  aa_eval_position;
out vec3  aa_eval_normal;
out vec4  aa_eval_color;
out float aa_eval_depth;

#define b300    gl_in[0].gl_Position.xyz
#define b030    gl_in[1].gl_Position.xyz
#define b003    gl_in[2].gl_Position.xyz
#define n200    aa_control_normal[0]
#define n020    aa_control_normal[1]
#define n002    aa_control_normal[2]
#define uvw     gl_TessCoord

void main()
{
  float u = uvw [0], u2 = u * u, u3 = u2 * u;
  float v = uvw [1], v2 = v * v, v3 = v2 * v;
  float w = uvw [2], w2 = w * w, w3 = w2 * w;

  vec3 b210 = vec3 (aa_control_patch[0].b210, aa_control_patch[1].b210, aa_control_patch[2].b210);
  vec3 b120 = vec3 (aa_control_patch[0].b120, aa_control_patch[1].b120, aa_control_patch[2].b120);
  vec3 b021 = vec3 (aa_control_patch[0].b021, aa_control_patch[1].b021, aa_control_patch[2].b021);
  vec3 b012 = vec3 (aa_control_patch[0].b012, aa_control_patch[1].b012, aa_control_patch[2].b012);
  vec3 b102 = vec3 (aa_control_patch[0].b102, aa_control_patch[1].b102, aa_control_patch[2].b102);
  vec3 b201 = vec3 (aa_control_patch[0].b201, aa_control_patch[1].b201, aa_control_patch[2].b201);
  vec3 b111 = vec3 (aa_control_patch[0].b111, aa_control_patch[1].b111, aa_control_patch[2].b111);

  vec3 n110 = normalize (vec3 (aa_control_patch[0].n110, aa_control_patch[1].n110, aa_control_patch[2].n110));
  vec3 n011 = normalize (vec3 (aa_control_patch[0].n011, aa_control_patch[1].n011, aa_control_patch[2].n011));
  vec3 n101 = normalize (vec3 (aa_control_patch[0].n101, aa_control_patch[1].n101, aa_control_patch[2].n101));

  vec4 color = gl_TessCoord[2] * aa_control_color[0]
             + gl_TessCoord[0] * aa_control_color[1]
             + gl_TessCoord[1] * aa_control_color[2];

  vec3 n1 = gl_TessCoord[2] * aa_control_normal[0]
          + gl_TessCoord[0] * aa_control_normal[1]
          + gl_TessCoord[1] * aa_control_normal[2];

  vec3 n2 = n200 * w2
          + n020 * u2
          + n002 * v2
          + n110 * w * u
          + n011 * u * v
          + n101 * w * v;

  vec3 normal = mix (n1, n2, aa_pn_triangles_weight);

  vec3 p1 = gl_TessCoord[2] * b300
          + gl_TessCoord[0] * b030
          + gl_TessCoord[1] * b003;

  vec3 p2 = b300 * w3
          + b030 * u3
          + b003 * v3
          + b210 * 3.0 * w2 * u
          + b120 * 3.0 * u2 * w
          + b201 * 3.0 * w2 * v
          + b021 * 3.0 * u2 * v
          + b102 * 3.0 * v2 * w
          + b012 * 3.0 * v2 * u
          + b111 * 6.0 * u * v * w;

  vec3 position = mix (p1, p2, aa_pn_triangles_weight);

  aa_eval_position = aa_gl_modelview     * vec4 (position, 1.0);
  aa_eval_normal   = aa_gl_normal_matrix * normal;
  aa_eval_color    = color;
  aa_eval_depth    = aa_gl_depth          (aa_eval_position);
  gl_Position      = aa_gl_projection    * aa_eval_position;
}
