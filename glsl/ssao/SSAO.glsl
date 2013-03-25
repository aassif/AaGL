#ifndef AA_SSAO
#define AA_SSAO

uniform mat4      aa_ssao_projection;
uniform vec2      aa_ssao_depth_range;
uniform bool      aa_ssao_randomized;
uniform uvec2     aa_ssao_screen_size;
uniform uvec2     aa_ssao_random_size;
uniform float     aa_ssao_radius;
uniform uint      aa_ssao_rings;
uniform uint      aa_ssao_slices;
uniform sampler2D aa_ssao_tex2d_color;
uniform sampler2D aa_ssao_tex2d_depth;
uniform sampler2D aa_ssao_tex2d_random;

const float PI = 3.1415926535897932384626433832795;

float aa_ssao_depth (float z)
{
  float n = aa_ssao_depth_range [0];
  float f = aa_ssao_depth_range [1];
  return n + z * (f - n);
}

float aa_ssao_random (vec2 p)
{
  if (! aa_ssao_randomized) return 0.0;
  vec2 v = (p * aa_ssao_screen_size) / aa_ssao_random_size;
  return texture (aa_ssao_tex2d_random, v).r;
}

float aa_ssao_occlusion (vec2 p, float z)
{
  float occlusion = 0.0;

  float z0 = aa_ssao_depth  (z);
  float i0 = aa_ssao_random (p);

  for (uint i = 0u; i < aa_ssao_slices; ++i)
  {
    for (uint j = 1u; j <= aa_ssao_rings; ++j)
    {
      //float a = (i + 0.5 * (j%2) + i0) * 2.0 * PI / aa_ssao_slices;
      float a = (i + i0) * 2.0 * PI / aa_ssao_slices;
      vec2  o = vec2 (cos (a), sin (a));
      float r = (j + i0) * aa_ssao_radius / (z0 * aa_ssao_rings);
      float z = aa_ssao_depth (texture (aa_ssao_tex2d_depth, p + r * o).r);
      occlusion += clamp ((z0 - z) / aa_ssao_radius, 0, 1);
    }
  }

  return 1.0 - occlusion / (aa_ssao_slices * aa_ssao_rings);
}

float aa_ssao_project (float z)
{
  return 0.5 * (1.0 - aa_ssao_projection[2][2] + aa_ssao_projection[3][2] / aa_ssao_depth (z));
}

#endif // AA_SSAO

