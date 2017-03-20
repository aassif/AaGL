#ifndef AA_GLSL_RAYTRACER__H
#define AA_GLSL_RAYTRACER__H

namespace Aa
{
  namespace GLSL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GLSL::Raytracer /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Raytracer :
      public TBasicProgram<ZeroUniform, ZeroUniform>
    {
      public:
        typedef TBasicProgram<ZeroUniform, ZeroUniform> Parent;

      public:
        enum Primitive {SPHERE = 0, CYLINDER = 1, CONE = 2};
        enum Lighting  {FILL   = 0, PHONG    = 1};

      private:
        Location<vec4> m_viewport;
        Location<mat4> m_modelview;
        Location<mat4> m_modelview_inv;
        Location<mat4> m_projection;
        Location<mat4> m_projection_inv;
        Location<vec4> m_color;

        SubroutineManager<2> m_subroutines;

      public:
        Raytracer () :
          Parent ("/Aa/Raytracer/Raytracer.vertex",
                  "/Aa/Raytracer/Raytracer.fragment"),
          m_subroutines (this,
                         GL_FRAGMENT_SHADER,
                         {
                           {"aa_raytracer_primitive", {"aa_raytracer_sphere", "aa_raytracer_cylinder", "aa_raytracer_cone"}},
                           {"aa_raytracer_lighting",  {"aa_raytracer_fill",   "aa_raytracer_phong"}}
                         })
        {
          m_viewport       = this->location ("aa_gl_viewport");
          m_modelview      = this->location ("aa_gl_modelview");
          m_modelview_inv  = this->location ("aa_gl_modelview_inv");
          m_projection     = this->location ("aa_gl_projection");
          m_projection_inv = this->location ("aa_gl_projection_inv");
          m_color          = this->location ("aa_gl_color");
        }

        using Parent::use;

        void use (AaUInt primitive, AaUInt lighting)
        {
          Program::use ();
          m_subroutines.set ({primitive, lighting});
        }

        void modelview (const mat4 & m)
        {
          m_modelview     (m);
          m_modelview_inv (m.inv ());
        }

        void projection (const mat4 & m)
        {
          m_projection     (m);
          m_projection_inv (m.inv ());

          ivec4 v;
          glGetIntegerv (GL_VIEWPORT, &(v[0]));
          m_viewport (v);
        }

        void color (const vec4 & c)
        {
          m_color (c);
        }
    };

  }
}

#include <AaMeshVAO>
#include <AaMeshPrimitives>
#include <AaMeshTransform>

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Raytracer ///////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    class Raytracer
    {
      public:
        typedef GLSL::Raytracer    Program;
        typedef Mesh::BasicMeshVAO VAO;

      public:
        enum Lighting
        {
          FILL  = GLSL::Raytracer::FILL,
          PHONG = GLSL::Raytracer::PHONG
        };

      private:
        inline static
        VAO * Impostor (const mat4 & m)
        {
          using namespace Mesh;
          std::unique_ptr<BasicMesh> cube (BasicCube::Create ());
          TMeshTransform<BasicMesh> transform (m);
          std::unique_ptr<BasicMesh> impostor (transform (cube.get ()));
          return new VAO (impostor.get ());
        }

      private:
        Program m_program;
        VAO * m_sphere;
        VAO * m_cylinder;
        VAO * m_cone;

      public:
        inline
        Raytracer () :
          m_program (),
          m_sphere   (Impostor (Translation (vec3 (-0.5f)))),
          m_cylinder (Impostor (Translation (vec3 {0.0f, -0.5f, -0.5f}))),
          m_cone     (Impostor (Translation (vec3 (-0.5f)) * Scale (vec3 {0.5f, 1.0f, 1.0f})))
        {
        }

        inline
        void projection (const mat4 & p)
        {
          m_program.use ();
          m_program.projection (p);
        }

        inline
        void sphere (const mat4 & m,
                     const vec3 & p, float r,
                     const vec4 & c,
                     enum Lighting mode = FILL)
        {
          m_program.use (GLSL::Raytracer::SPHERE, mode);
          m_program.modelview (m * Translation (p) * Scale (r));
          m_program.color (c);
          m_sphere->draw ();
        }

        inline
        void cylinder (const mat4 & m,
                       const vec3 & p1, const vec3 & p2, float r,
                       const vec4 & c,
                       enum Lighting mode = FILL)
        {
          vec3 d = p2 - p1;
          mat4 T = Translation (p1);
          mat4 R = Transform ({1, 0, 0}, d);
          mat4 S = Scale (vec<float> (d.length (), r, r));
          m_program.use (GLSL::Raytracer::CYLINDER, mode);
          m_program.modelview (m * T * R * S);
          m_program.color (c);
          m_cylinder->draw ();
        }

        inline
        void cone (const mat4 & m,
                   const vec3 & p1,
                   const vec3 & p2, float r,
                   const vec4 & c,
                   enum Lighting mode = FILL)
        {
          vec3 d = p2 - p1;
          mat4 T = Translation (p2);
          mat4 R = Transform ({1, 0, 0}, d);
          mat4 S = Scale (vec<float> (2.0 * d.length (), r, r));
          m_program.use (GLSL::Raytracer::CONE, mode);
          m_program.modelview (m * T * R * S);
          m_program.color (c);
          m_cone->draw ();
        }

        inline
        void arrow (const mat4 & m,
                    const vec3 & p1,
                    const vec3 & p2,
                    float radius,
                    const vec4 & c,
                    enum Lighting mode = FILL)
        {
          vec3 d = p2 - p1;
          float l = d.length ();
          if (l > 0)
          {
            float r = std::min (0.5f * l, radius);
            vec3 p3 = p1 + (l - 2 * r) * (d / l);
            this->cylinder (m, p1, p3, r, c, mode);
            this->cone (m, p3, p2, 2 * r, c, mode);
          }
        }

        inline
        void frame (const mat4 & m, enum Lighting mode = FILL)
        {
          this->sphere (m, vec3 (0),            0.2f, {0, 0, 0, 1}, mode);
          this->arrow  (m, vec3 (0), {1, 0, 0}, 0.1f, {1, 0, 0, 1}, mode);
          this->arrow  (m, vec3 (0), {0, 1, 0}, 0.1f, {0, 1, 0, 1}, mode);
          this->arrow  (m, vec3 (0), {0, 0, 1}, 0.1f, {0, 0, 1, 1}, mode);
        }
    };
  }
}

#endif // AA_GLSL_RAYTRACER__H

