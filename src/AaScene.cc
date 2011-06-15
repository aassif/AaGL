#include <cmath>
#include <sstream>
#include <iomanip>
//#include <GL/glew.h>
//#include <GL/glut.h>
#include <AaTimer>
#include "AaScene"

//#define AA_GL_UPSIDE_DOWN

using namespace std;
//using namespace Aa;

namespace Aa
{
  namespace GL
  {

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Plugin //////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    Plugin::~Plugin () {}

    void Plugin::glSelect (int /* mode */)
    {
      //cout << "Plugin::glSelect (" << mode << ");\n";
    }
    
    void Plugin::glSelect (const vector<GLuint> & /* stack */, bool /* multiple */)
    {
#if 0
      cout << "Plugin::glSelect (";
      if (! stack.empty ())
      {
        vector<GLuint>::const_iterator
          i = stack.begin (), e = stack.end ();
        cout << *(i++); while (i != e) cout << ", " << *(i++);
      }
      cout << ", " << multiple << ");\n";
#endif
    }

    void Plugin::glToolTip (const vector<GLuint> & /* stack */, double /* x */, double /* y */)
    {
#if 0
      cout << "Plugin::glToolTip (";
      if (! stack.empty ())
      {
        vector<GLuint>::const_iterator
          i = stack.begin (), e = stack.end ();
        cout << *(i++); while (i != e) cout << ", " << *(i++);
      }
      cout << ", " << x << ", " << y << ");\n";
#endif
    }

    void Plugin::glPopup (const vector<GLuint> & /* stack */, double /* x */, double /* y */)
    {
#if 0
      cout << "Plugin::glPopup (";
      if (! stack.empty ())
      {
        vector<GLuint>::const_iterator
          i = stack.begin (), e = stack.end ();
        cout << *(i++); while (i != e) cout << ", " << *(i++);
      }
      cout << ", " << x << ", " << y << ");\n";
#endif
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::SelectionBox ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    SelectionBox::SelectionBox (const Scene * s, double x1, double y1) :
      m_scene (s),
      m_x1 (x1), m_y1 (y1),
      m_x2 (x1), m_y2 (y1) {}

    void SelectionBox::setCorner (double x2, double y2) {m_x2 = x2; m_y2 = y2;}

    int SelectionBox::glSelect () const
    {
      double x = (m_x1 + m_x2) / 2.0, w = std::max (m_x1 < m_x2 ? m_x2 - m_x1 : m_x1 - m_x2, 1.0);
      double y = (m_y1 + m_y2) / 2.0, h = std::max (m_y1 < m_y2 ? m_y2 - m_y1 : m_y1 - m_y2, 1.0);
      return m_scene->glPick (x, y, w, h);
    }

    void SelectionBox::glDraw (bool /* motion */)
    {
      glPushAttrib (GL_POLYGON_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable (GL_DEPTH_TEST);

      int viewport [4];
      glGetIntegerv (GL_VIEWPORT, viewport);

      glMatrixMode (GL_PROJECTION);
      glPushMatrix ();
      glLoadIdentity ();
      glOrtho (0.0, viewport [2] , 0.0, viewport [3], 0.0, 1.0);

      glMatrixMode (GL_MODELVIEW);
      glPushMatrix ();
      glLoadIdentity ();

      glEnable (GL_BLEND);
      glDisable (GL_LIGHTING);
      glDisable (GL_CULL_FACE);
      static const float m_fill_color [] = {0.0, 1.0, 0.0, 0.10};
      glColor4fv (m_fill_color);
      glRectd (m_x1, m_y1, m_x2, m_y2);

      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      static const float m_outline_color [] = {0.0, 1.0, 0.0, 0.50};
      glColor4fv (m_outline_color);
      glRectd (m_x1, m_y1, m_x2, m_y2);

      glMatrixMode (GL_PROJECTION);
      glPopMatrix ();

      glMatrixMode (GL_MODELVIEW);
      glPopMatrix ();

      glPopAttrib ();

      // This part needs an exclusive lock because glSelect calls glDraw.
      static bool lock = false;
      if (! lock) {lock = true; this->glSelect (); lock = false;}
    }

////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Scene ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    GLuint Scene::PickId (const void * p)
    {
#if 1
      return 0;
#else
      return (GLuint) p;
#endif
    }

    void * Scene::PickPtr (GLuint id)
    {
#if 1
      return NULL;
#else
      return (void *) id;
#endif
    }

    Scene::Scene (const Math::pR3 & c, const Math::pR3 & t) :
      m_camera (c), m_target (t),
      m_m3d_view (), m_m3d_proj (),
      m_motion (MOTION_NONE),
      m_plugins (),
      m_title ()
    {
      // Cameras.
      this->updateViewMatrix ();

      // Z-buffer.
      glDepthFunc (GL_LESS);
      glEnable (GL_DEPTH_TEST);

      // Culling.
      glCullFace (GL_BACK);
      glEnable (GL_CULL_FACE);
      //glDisable (GL_CULL_FACE);

      // Blending.
      glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glDisable (GL_BLEND);

      // Background color.
      glClearColor (1.0, 1.0, 1.0, 0.0);
      //glClearColor (0.0, 0.0, 0.0, 0.0);

      // Line width.
      glLineWidth (1.5);

      // Fog.
      //static float COLOR_FOG [] = {1.0, 1.0, 1.0, 1.0};
      //glFogi (GL_FOG_MODE, GL_EXP);
      //glFogf (GL_FOG_DENSITY, 0.005);
      //glFogf (GL_FOG_START,     1.0);
      //glFogf (GL_FOG_END,   65536.0);
      //glFogfv (GL_FOG_COLOR, COLOR_FOG);
    }

    Scene::~Scene ()
    {
    }

    void Scene::updateViewMatrix ()
    {
      glMatrixMode (GL_MODELVIEW);
      glLoadIdentity ();
#ifdef AA_GL_UPSIDE_DOWN
      gluLookAt (m_camera.x, m_camera.y, m_camera.z, m_target.x, m_target.y, m_target.z, 0, -1, 0);
#else
      gluLookAt (m_camera.x, m_camera.y, m_camera.z, m_target.x, m_target.y, m_target.z, 0, 1, 0);
#endif
      glGetDoublev (GL_MODELVIEW_MATRIX, m_m3d_view);
    }

    // Pseudo-trackball.

    void Scene::setCamera (const Math::pR3 & p)
    {
      m_camera = p;
      this->updateViewMatrix ();
    }

    const Math::pR3 & Scene::getCamera () const {return m_camera;}

    void Scene::setTarget (const Math::pR3 & p)
    {
      m_target = p;
      this->updateViewMatrix ();
    }

    const Math::pR3 & Scene::getTarget () const {return m_target;}

    void Scene::setMotionMode (int mode)
    {
      m_motion = mode;
    }

    void Scene::targetRotate (int dx, int dy)
    {
      Math::vR3 v (m_camera, m_target);
      double theta, phi, rho; Math::ToPolar (Math::pR3 (v.x, v.y, v.z), &theta, &phi, &rho);

      if (dx != 0) theta -= ((double) dx * M_PI / 180.0);
      if (dy != 0) phi   += ((double) dy * M_PI / 180.0);
      if (phi > M_PI_2 - 0.01) phi = M_PI_2 - 0.01;
      else if (phi < -M_PI_2 + 0.01) phi = -M_PI_2 + 0.01;

      Math::pR3 p = Math::FromPolar (theta, phi, rho); v = Math::vR3 (p.x, p.y, p.z);
      m_target = m_camera + v;
      this->updateViewMatrix ();
      //glutPostRedisplay ();
    }

    void Scene::cameraRotate (int dx, int dy)
    {
      Math::vR3 v (m_target, m_camera);
      double theta, phi, rho; Math::ToPolar (Math::pR3 (v.x, v.y, v.z), &theta, &phi, &rho);

      if (dx != 0) theta -= ((double) dx * M_PI / 180.0);
      if (dy != 0) phi   -= ((double) dy * M_PI / 180.0);
      if (phi > M_PI_2 - 0.01) phi = M_PI_2 - 0.01;
      else if (phi < -M_PI_2 + 0.01) phi = -M_PI_2 + 0.01;

      Math::pR3 p = Math::FromPolar (theta, phi, rho); v = Math::vR3 (p.x, p.y, p.z);
      m_camera = m_target + v;
      this->updateViewMatrix ();
      //glutPostRedisplay ();
    }

    void Scene::cameraStrafe (int dx, int dy)
    {
      Math::vR3 v (dx, dy, 0.0);
      Math::vR3 strafe (v % Math::vR3 (m_m3d_view [0], m_m3d_view [1], m_m3d_view  [2]),
                        v % Math::vR3 (m_m3d_view [4], m_m3d_view [5], m_m3d_view  [6]),
                        v % Math::vR3 (m_m3d_view [8], m_m3d_view [9], m_m3d_view [10]));
      m_camera += strafe;
      m_target += strafe;
      this->updateViewMatrix ();
      //glutPostRedisplay ();
    }

    void Scene::cameraForward (int dy)
    {
      Math::vR3 v (m_target, m_camera);
      double theta, phi, rho; Math::ToPolar (Math::pR3 (v.x, v.y, v.z), &theta, &phi, &rho);

      if (dy != 0) rho *= std::pow (1.1, dy);
      //if (rho < 0.01) rho = 0.01;

      Math::pR3 p = Math::FromPolar (theta, phi, rho); v = Math::vR3 (p.x, p.y, p.z);
      m_camera = m_target + v;
      this->updateViewMatrix ();
      //glutPostRedisplay ();
    }

    void Scene::motion (int dx, int dy)
    {
#ifdef AA_GL_UPSIDE_DOWN
      switch (m_motion)
      {
      case MOTION_TARGET_ROTATE:  this->targetRotate (dx, dy); break;
      case MOTION_CAMERA_ROTATE:  this->cameraRotate (dx, dy); break;
      case MOTION_CAMERA_STRAFE:  this->cameraStrafe (dx, dy); break;
      case MOTION_CAMERA_FORWARD: this->cameraForward (dy); break;
      }
#else
      switch (m_motion)
      {
      case MOTION_TARGET_ROTATE:  this->targetRotate (-dx, -dy); break;
      case MOTION_CAMERA_ROTATE:  this->cameraRotate (-dx, -dy); break;
      case MOTION_CAMERA_STRAFE:  this->cameraStrafe (-dx, -dy); break;
      case MOTION_CAMERA_FORWARD: this->cameraForward (dy); break;
      }
#endif
    }

    void Scene::glReshape (int w, int h)
    {
      glViewport (0, 0, w, h);

      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      GLdouble zNear = 1.0, zFar = 4096.0;
      gluPerspective (30.0, (GLdouble) w / (GLdouble) h, zNear, zFar);
      glGetDoublev (GL_PROJECTION_MATRIX, m_m3d_proj);

      glMatrixMode (GL_MODELVIEW);
    }

    void Scene::glDraw () const
    {
      glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      glMatrixMode (GL_PROJECTION);
      glLoadMatrixd (m_m3d_proj);

      glMatrixMode (GL_MODELVIEW);
      glLoadMatrixd (m_m3d_view);

      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

      // Lighting.
      //static const GLfloat light0_ambient  [] = {1.0, 1.0, 1.0, 1.0};
      static const GLfloat light0_diffuse  [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light0_specular [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light0_position [] = {-10, -10, -10, 0.0};
            
      //glLightfv (GL_LIGHT0, GL_AMBIENT,  light0_ambient);
      glLightfv (GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
      glLightfv (GL_LIGHT0, GL_SPECULAR, light0_specular);
      glLightfv (GL_LIGHT0, GL_POSITION, light0_position);
      glEnable  (GL_LIGHT0);

      //static const GLfloat light1_ambient  [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light1_diffuse  [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light1_specular [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light1_position [] = {+10, -10, -10, 0.0};

      //glLightfv (GL_LIGHT1, GL_AMBIENT,  light1_ambient);
      glLightfv (GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
      glLightfv (GL_LIGHT1, GL_SPECULAR, light1_specular);
      glLightfv (GL_LIGHT1, GL_POSITION, light1_position);
      glEnable  (GL_LIGHT1);

      //  static const GLfloat light2_ambient  [] = {1.0, 1.0, 1.0, 1.0};
      static const GLfloat light2_diffuse  [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light2_specular [] = {0.5, 0.5, 0.5, 1.0};
      static const GLfloat light2_position [] = {  0, -10, +10, 0.0};
            
      //glLightfv (GL_LIGHT0, GL_AMBIENT,  light2_ambient);
      glLightfv (GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
      glLightfv (GL_LIGHT2, GL_SPECULAR, light2_specular);
      glLightfv (GL_LIGHT2, GL_POSITION, light2_position);
      glEnable  (GL_LIGHT2);

      static const GLfloat lmodel_ambient [] = {1.0, 1.0, 1.0, 1.0};
      glLightModelfv (GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

      // Smooth or flat.
      glShadeModel (GL_SMOOTH);

      static Aa::Timer timer = Aa::Timer (true);
      static int nFrames = 0;
      static string msgFps = "N/A";

      for (list<Plugin *>::const_iterator
             i = m_plugins.begin (), e = m_plugins.end (); i != e;)
      {
        Plugin * plugin = *(i++);
        plugin->glDraw (m_motion != MOTION_NONE);
      }

      GLenum e = glGetError ();
      if (e != GL_NO_ERROR) cerr << gluErrorString (e) << endl;

      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      GLint viewport [4];
      glGetIntegerv (GL_VIEWPORT, viewport);
      glOrtho (0.0, viewport [2], 0.0, viewport [3], 0.0, 1.0);

      glMatrixMode (GL_MODELVIEW);
      glLoadIdentity ();

      glDepthMask (GL_FALSE);
#if 0
      static const float FPS_COLOR [] = {0.0, 0.0, 0.0, 1.0};
      Lighting::color (FPS_COLOR);
      Text::write (4, 4, msgFps, GLUT_BITMAP_HELVETICA_18);
      //ostringstream oss;
      //oss << nFrames;
      //Text::write (4, 20, oss.str (), GLUT_BITMAP_HELVETICA_18);
#endif
      //Menu::GlDraw ();
      glDepthMask (GL_TRUE);

      glFinish ();
      glFlush ();
      
      //glutSwapBuffers ();

      ++nFrames;
      double seconds = timer.get ();
      if (seconds > 1.0)
      {
        double seconds = timer.get ();
        double fps = nFrames / seconds;
        double ms  = 1000.0 * seconds / nFrames;
        ostringstream oss;
        oss << m_title << " - " << fixed << setprecision (3) << fps << " fps / " << ms << " ms";
        msgFps = oss.str ();
        timer.start ();
        nFrames = 0;
      }
    }

    void Scene::glSelect (bool replace) const
    {
      for (list<Plugin *>::const_iterator
             i = m_plugins.begin (), e = m_plugins.end (); i != e;)
      {
        Plugin * plugin = *(i++);
        plugin->glSelect (replace ? 2 : 0);
      }
    }

    int Scene::glPick (double x, double y, double w, double h, int action) const
    {
#if 0
      cout << "Scene::glSelect ("
           << "x = " << x << ", y = " << y << ", "
           << "w = " << w << ", h = " << h << ", "
           << "multiple = " << multiple << ", popup = " << popup
           << ");\n";
#endif

      if (action == PICK_SWEEP)
      {
        for (list<Plugin *>::const_iterator
               i = m_plugins.begin (), e = m_plugins.end (); i != e;)
        {
          Plugin * plugin = *(i++);
          plugin->glSelect (1);
        }
      }

      int r = 0;
      GLint viewport [4];
      glGetIntegerv (GL_VIEWPORT, viewport);

      // Draw scene in GL_SELECT mode.
      GLuint selectBuffer [1024];
      glSelectBuffer (1024, selectBuffer);
      glRenderMode (GL_SELECT);

      // Picking matrix.
      glMatrixMode (GL_PROJECTION);
      glLoadIdentity ();
      gluPickMatrix (x, y, w, h, viewport);
      glMultMatrixd (m_m3d_proj);

      glMatrixMode (GL_MODELVIEW);
      glLoadMatrixd (m_m3d_view);

      glInitNames ();
      for (list<Plugin *>::const_iterator
             i = m_plugins.begin (), e = m_plugins.end (); i != e;)
      {
        Plugin * plugin = *(i++);
        glPushName (PickId (plugin));
        plugin->glDraw (true);
        glPopName ();
      }
      GLint hits = glRenderMode (GL_RENDER);

      // Pick event.
      Plugin * plugin = NULL;
      double z = 2.0; vector<GLuint> stack;

      GLuint * selectPointer = selectBuffer;
      for (GLint i = 0; i < hits; ++i)
      {
        GLuint n = selectPointer [0];
        double z1 = (double) selectPointer [1] / 0xFFFFFFFF;
        //double z2 = (double) selectPointer [2] / 0xFFFFFFFF;
        selectPointer += 3;
        if (n > 1)
        {
          if (action == PICK_SWEEP)
          {
            stack.assign (selectPointer + 1, selectPointer + n);
            plugin = (Plugin *) PickPtr (selectPointer [0]);
            plugin->glSelect (stack, true); ++r;
          }
          else if (z1 < z)
          {
            z = z1;
            stack.assign (selectPointer + 1, selectPointer + n);
            plugin = (Plugin *) PickPtr (selectPointer [0]);
          }
        }
        selectPointer += n;
      }  

      if (plugin != NULL)
      {
        switch (action)
        {
          case PICK_SELECT:
            plugin->glSelect (stack, false);
            return 1;

          case PICK_POPUP:
            plugin->glPopup (stack, x, y);
            return 1;

          case PICK_TOOLTIP:
            plugin->glToolTip (stack, x, y);
            return 1;

          default:
            break;
        }
      }

      return r;
    }

    int Scene::glPick (double x, double y, int action) const
    {
      return this->glPick (x, y, 1, 1, action);
    }

    int Scene::glPick (double x, double y, double w, double h) const
    {
      return this->glPick (x, y, w, h, PICK_SWEEP);
    }

    void Scene::addInput (Plugin * plugin)
    {
      m_plugins.push_back (plugin);
    }

    void Scene::removeInput (Plugin * plugin)
    {
      m_plugins.remove (plugin);
    }

    void Scene::setTitle (const string & str)
    {
      m_title = str;
    }

    void Scene::setBackground (double r, double g, double b)
    {
      glClearColor (r, g, b, 0.0);
    }

  } // namespace GL
} // namespace Aa

