#ifndef __AA_GL_SCENE__
#define __AA_GL_SCENE__

//#include <string>
//#include <set>
#include <vector>
#include <list>
#include <GL/glew.h>
#include <AaMath>

namespace Aa
{
  namespace GL
  {

    // Scene input.
    class AA_GL_API Plugin
    {
      public:
        virtual ~Plugin ();
        // OpenGL picking.
        virtual void glSelect  (int mode);
        virtual void glSelect  (const std::vector<GLuint> &, bool multiple);
        virtual void glToolTip (const std::vector<GLuint> &, double x, double y);
        virtual void glPopup   (const std::vector<GLuint> &, double x, double y);
        // OpenGL display.
        virtual void glDraw (bool motion = false) = 0;
    };

    // Selection.
    class Scene;
    class AA_GL_API SelectionBox : public Plugin
    {
      protected:
        const Scene * m_scene;
        double m_x1, m_y1;
        double m_x2, m_y2;
        // OpenGL picking.
        int glSelect () const;

      public:
        SelectionBox (const Scene *, double x1, double y1);
        void setCorner (double x2, double y2);
        void glDraw (bool motion);
    };

    // Scene.
    class AA_GL_API Scene
    {
      public:
        // Motion.
        enum
        {
          MOTION_NONE           = 0,
          MOTION_TARGET_ROTATE  = 1,
          MOTION_CAMERA_ROTATE  = 2,
          MOTION_CAMERA_FORWARD = 3,
          MOTION_CAMERA_STRAFE  = 4
        };

        // Mouse buttons.
        enum
        {
          BUTTON_LEFT   = 0,
          BUTTON_MIDDLE = 1,
          BUTTON_RIGHT  = 2
        };

        // Picking modes.
        enum
        {
          PICK_SELECT  = 0,
          PICK_SWEEP   = 1,
          PICK_POPUP   = 2,
          PICK_TOOLTIP = 3
        };

      public:
        static GLuint PickId  (const void *);
        static void * PickPtr (GLuint);

      private:
        // Matrices.
        dvec3 m_camera;
        dvec3 m_target;
        double m_m3d_view [16];
        double m_m3d_proj [16];
        // Motion mode.
        int m_motion;
        // The plugins.
        std::list<Plugin *> m_plugins;
        // Scene title.
        std::string m_title;

      private:
        // Matrix update.
        void updateViewMatrix ();
        void updateProjMatrix ();

        // Pseudo-trackball.
        void targetRotate (int dx, int dy);
        void cameraRotate (int dx, int dy);
        void cameraStrafe (int dx, int dy);
        void cameraForward (int dy);

        // OpenGL picking.
        int glPick (double x, double y, double w, double h, int action) const;

      public:
        // Constructor.
        Scene (const dvec3 & camera = vec (0.0, 0.0, 1.0),
               const dvec3 & target = vec (0.0, 0.0, 0.0));
        // Destructor.
        ~Scene ();

        // Pseudo-trackball.
        void setCamera (const dvec3 &);
        const dvec3 & getCamera () const;
        void setTarget (const dvec3 &);
        const dvec3 & getTarget () const;
        void setMotionMode (int);
        void motion (int dx, int dy);

        // Add an input.
        void addInput (Plugin *);
        void removeInput (Plugin *);

        // OpenGL display.
        void glReshape (int w, int h);
        void glDraw () const;

        // OpenGL picking.
        void glSelect (bool replace) const;
        int glPick (double x, double y, int action) const;
        int glPick (double x, double y, double w, double h) const;

        // Title.
        void setTitle (const std::string &);
        // Background.
        void setBackground (double r, double g, double b);
    };
  }
}

#endif // __AA_GL_SCENE__
