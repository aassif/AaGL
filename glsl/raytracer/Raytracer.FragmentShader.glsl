#version 410 core

uniform vec4 aa_gl_viewport;
uniform mat4 aa_gl_modelview;
uniform mat4 aa_gl_modelview_inv;
uniform mat4 aa_gl_projection;
uniform mat4 aa_gl_projection_inv;
uniform vec4 aa_gl_color;

in vec3 aa_vertex_position;

layout (location = 0) out vec4 aa_raytracer_color;

struct Ray
{
  vec3 origin;
  vec3 direction;
};

vec3 aa_raytracer_origin ()
{
  vec2 xy = 2 * (gl_FragCoord.xy - aa_gl_viewport.xy) / aa_gl_viewport.zw - 1;
  vec4 p = aa_gl_projection_inv * vec4 (xy, -1, 1);
  return (aa_gl_modelview_inv * (p / p.w)).xyz;
}

Ray aa_raytracer_ray ()
{
  vec3 o = aa_raytracer_origin ();
  vec3 r = normalize (aa_vertex_position - o);
  return Ray (o, r);
}

float aa_raytracer_depth (in vec3 p)
{
  float n = gl_DepthRange.near;
  float f = gl_DepthRange.far;
  vec4 q = aa_gl_projection * aa_gl_modelview * vec4 (p, 1);
  return ((n + f) + (q.z / q.w) * (f - n)) / 2;
}

struct Plane
{
  vec3 point;
  vec3 normal;
};

struct Intersection
{
  vec3 position;
  vec3 normal;
};

Intersection aa_raytracer_disk (in Ray r, in vec3 c, in vec3 n, in float radius)
{
  vec3 d = r.origin - c;
  float t = - dot (d, n) / dot (r.direction, n);
  vec3 p = r.origin + t * r.direction;
  if (length (p - c) > radius) discard;
  return Intersection (p, n);
}

subroutine
Intersection RaytracerPrimitive (in Ray);

//layout (index = 0)
subroutine (RaytracerPrimitive)
Intersection aa_raytracer_sphere (in Ray ray)
{
  const float R = 0.5;

  vec3 o = ray.origin;
  vec3 r = ray.direction;

  float k1 = dot (o, r);
  float k2 = dot (o, o) - (R * R);
  float k3 = (k1 * k1) - k2;

  if (k3 < 0.0) discard;

  float t = -k1 - sqrt (k3);
  if (t < 0.0) discard;

  vec3 p = o + r * t;
  vec3 n = normalize (p);
  return Intersection (p, n);
}

//layout (index = 1)
subroutine (RaytracerPrimitive)
Intersection aa_raytracer_cylinder (in Ray ray)
{
  const float R = 0.5;

  vec3 o = ray.origin;
  vec3 r = ray.direction;

  float k0 = dot (r.yz, r.yz);           // a
  float k1 = dot (o.yz, r.yz);           // b/2
  float k2 = dot (o.yz, o.yz) - (R * R); // c
  float k3 = (k1 * k1) - k0 * k2;        // d/4

  if (k3 < 0.0) discard;

  float t = - (k1 + sqrt (k3)) / k0;
  if (t < 0.0) discard;

  vec3 p = o + r * t;

  if (p.x < 0)
  {
    const vec3 C = vec3 ( 0, 0, 0);
    const vec3 N = vec3 (-1, 0, 0);
    return aa_raytracer_disk (ray, C, N, R);
  }

  if (p.x > 1)
  {
    const vec3 C = vec3 (+1, 0, 0);
    const vec3 N = vec3 (+1, 0, 0);
    return aa_raytracer_disk (ray, C, N, R);
  }

  vec3 n = vec3 (0, normalize (p.yz));
  return Intersection (p, n);
}

//layout (index = 2)
subroutine (RaytracerPrimitive)
Intersection aa_raytracer_cone (in Ray ray)
{
  const float R = 0.5;

  vec3 o = ray.origin;
  vec3 r = ray.direction;

  float k0 = dot (r.yz, r.yz) - (r.x * r.x); // a
  float k1 = dot (o.yz, r.yz) - (o.x * r.x); // b/2
  float k2 = dot (o.yz, o.yz) - (o.x * o.x); // c
  float k3 = (k1 * k1) - k0 * k2;            // d/4

  if (k3 < 0.0) discard;

  float t = - (k1 + sqrt (k3)) / k0;
  if (t < 0.0) discard;

  vec3 p = o + r * t;

  if (p.x > 0.0) discard;

  if (p.x < -0.5)
  {
    const vec3 C = vec3 (-0.5, 0, 0);
    const vec3 N = vec3 (-1.0, 0, 0);
    return aa_raytracer_disk (ray, C, N, R);
  }

  vec3 n = normalize (vec3 (1, normalize (p.yz)));
  return Intersection (p, n);
}

subroutine
vec4 RaytracerLighting (in Ray, in Intersection);

subroutine (RaytracerLighting)
vec4 aa_raytracer_fill (in Ray, in Intersection)
{
  return aa_gl_color;
}

subroutine (RaytracerLighting)
vec4 aa_raytracer_phong (in Ray ray, in Intersection i)
{
  const float AMBIENT = 0.5;
  const float SHININESS = 16.0;
  const vec3  WHITE = vec3 (0.75);
  mat3 m0 = mat3 (aa_gl_modelview);
  mat3 m1 = transpose (mat3 (aa_gl_modelview_inv));
  vec3 n = normalize (m1 * i.normal);
  //vec3 l = normalize (light - vec3 (aa_gl_modelview * vec4 (i.position, 1)));
  vec3 l = normalize (vec3 (1, 1, 1));
  vec3 r = reflect (l, n);
  vec3 v = normalize (m0 * ray.direction);
  float diffuse = max (dot (l, n), 0.0);
  float phong = pow (max (dot (v, r), 0.0), SHININESS);
  return vec4 (aa_gl_color.rgb * mix (AMBIENT, 1.0, diffuse) + WHITE * phong, aa_gl_color.a);
}

//layout (location = 0)
subroutine uniform RaytracerPrimitive aa_raytracer_primitive;

//layout (location = 1)
subroutine uniform RaytracerLighting aa_raytracer_lighting;

void main ()
{
  Ray ray = aa_raytracer_ray ();
  Intersection i = aa_raytracer_primitive (ray);
  aa_raytracer_color = aa_raytracer_lighting (ray, i);
  gl_FragDepth = aa_raytracer_depth (i.position);
}
