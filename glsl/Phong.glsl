uniform float aa_phong_diffuse   = 0.25;
uniform float aa_phong_specular  = 0.5;
uniform float aa_phong_shininess = 16;

const vec3 EYE   = normalize (vec3 (0, 0, 1));
const vec3 LIGHT = normalize (vec3 (1, 1, 1));

vec4 aa_phong (vec3 n, vec4 c)
{
  vec3  normal   = normalize (n);
  float diffuse  = max (0, dot (LIGHT, normal));
  float specular = pow (max (0, dot (reflect (LIGHT, normal), -EYE)), aa_phong_shininess);
  return c * mix (1.0, diffuse, aa_phong_diffuse) + specular * aa_phong_specular;
}

