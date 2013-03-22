#ifndef AA_PHONG
#define AA_PHONG

uniform float aa_phong_ambient   = 0.0;
uniform float aa_phong_diffuse   = 1.0;
uniform float aa_phong_specular  = 0.5;
uniform float aa_phong_shininess = 16;

const vec3 AA_PHONG_EYE   = normalize (vec3 (0, 0, 1));
const vec3 AA_PHONG_LIGHT = normalize (vec3 (0, 0, 1));

vec4 aa_phong (vec3 n, vec4 c, float white = 1)
{
  vec3  normal   = normalize (n);
  float diffuse  = max (0, dot (AA_PHONG_LIGHT, normal));
  float specular = pow (max (0, dot (reflect (-AA_PHONG_LIGHT, normal), AA_PHONG_EYE)), aa_phong_shininess);
  return vec4 (c.rgb * (aa_phong_ambient + diffuse * aa_phong_diffuse) + vec3 (white * specular * aa_phong_specular), c.a);
}

#endif // AA_PHONG

