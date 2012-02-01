#ifndef __AA_GLUT_MENU__
#define __AA_GLUT_MENU__

#include <vector>

namespace Aa
{
  namespace GL
  {

//////////////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Menu //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

    class AA_GL_API Menu
    {
      protected:
        static double s_x, s_y;
        static Menu * s_active;

      public:
        static void Enable (Menu *, double x, double y);
        static void Disable ();
        static bool Click (double x, double y, int button);
        static void GlDraw ();

      public:
        virtual ~Menu ();
        void popup (double x, double y);
        virtual bool click (double x, double y, int button) = 0;
        virtual void glMenu () const = 0;
    };

//////////////////////////////////////////////////////////////////////////////////////////
// Aa::GL::SimpleMenu ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

    class AA_GL_API SimpleMenu : public Menu
    {
      protected:
        static const int MARGIN_LEFT;
        static const int MARGIN_RIGHT;
        static const int MARGIN_TOP;
        static const int MARGIN_BOTTOM;
        static const int SEPARATOR;

      protected:
        void * m_font;
        int m_width, m_height;
        std::string m_title;
        std::vector<std::string> m_items;

      protected:
        virtual bool click (int) = 0;

      public:
        SimpleMenu (const std::string & title,
                    void * font = GLUT_BITMAP_HELVETICA_18, int = 20);
        virtual ~SimpleMenu ();
        virtual void addItem (const std::string & item);
        virtual bool click (double x, double y, int button);
        virtual void glMenu () const;
    };

//////////////////////////////////////////////////////////////////////////////////////////
// Aa::GL::Menu //////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

    double Menu::s_x = -1.0;
    double Menu::s_y = -1.0;
    Menu * Menu::s_active = NULL;

    void Menu::Enable (Menu * m, double x, double y)
    {
      s_x = x;
      s_y = y;
      s_active = m;
    }

    void Menu::Disable ()
    {
      s_active = NULL;
    }

    bool Menu::Click (double x, double y, int button)
    {
      if (s_active != NULL)
      {
        bool caught = s_active->click (x - s_x, y - s_y, button);
        if (! caught) Menu::Disable ();
        return caught;
      }

      return false;
    }

    void Menu::GlDraw ()
    {
      if (s_active != NULL)
      {
        glTranslated (s_x, s_y, 0.0);
        s_active->glMenu ();
      }
    }

    Menu::~Menu () {}

    void Menu::popup (double x, double y)
    {
      Menu::Enable (this, x, y);
    }

//////////////////////////////////////////////////////////////////////////////////////////
// Aa::GL::SimpleMenu ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

    const int SimpleMenu::MARGIN_LEFT   = 4;
    const int SimpleMenu::MARGIN_RIGHT  = 4;
    const int SimpleMenu::MARGIN_TOP    = 4;
    const int SimpleMenu::MARGIN_BOTTOM = 4;
    const int SimpleMenu::SEPARATOR     = 8;

    SimpleMenu::SimpleMenu (const string & title, void * font, int h) :
      Menu (),
      m_font (font),
      m_width (0),
      m_height (h),
      m_title (title),
      m_items ()
    {
    }

    SimpleMenu::~SimpleMenu () {}

    void SimpleMenu::addItem (const string & item)
    {
      m_items.push_back (item);
      int w = Text::Width (item, m_font);
      if (w > m_width) m_width = w;
    }

    bool SimpleMenu::click (double x, double y, int /* button */)
    {
      int n = (int) std::floor ((double) (-y - (MARGIN_TOP + m_height + MARGIN_BOTTOM + SEPARATOR + MARGIN_TOP)) / m_height);
      if (x < 0 || x > m_width) return false;
      if (n < 0 || n >= (int) m_items.size ()) return false;
      if (this->click (n)) Menu::Disable ();
      return true;
    }

    void SimpleMenu::glMenu () const
    {
      glDisable (GL_LIGHTING);
      // Backup state.
      GLint backup_glPolygonMode [2];
      glGetIntegerv (GL_POLYGON_MODE, backup_glPolygonMode);
      GLfloat backup_glLineWidth;
      glGetFloatv (GL_LINE_WIDTH, &backup_glLineWidth);
      GLboolean backup_glCullFace = glIsEnabled (GL_CULL_FACE);
      glDisable (GL_CULL_FACE);
      glScalef (1.0, -1.0, 1.0);
      // Title.
      glEnable (GL_BLEND);
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glColor4f (0.9, 0.9, 0.9, 0.75);
      glRecti (0, 0, (MARGIN_LEFT + m_width + MARGIN_RIGHT), (MARGIN_TOP + m_height + MARGIN_BOTTOM));
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth (2.0);
      glColor4f (0.8, 0.8, 0.8, 0.75);
      glRecti (0, 0, (MARGIN_LEFT + m_width + MARGIN_RIGHT), (MARGIN_TOP + m_height + MARGIN_BOTTOM));
      glColor4f (0.0, 0.0, 0.0, 1.0);
      Text::Write (MARGIN_LEFT, (MARGIN_TOP + m_height), m_title);
      // Items.
      int n = m_items.size ();
      glTranslatef (0.0, (MARGIN_TOP + m_height + MARGIN_BOTTOM + SEPARATOR), 0.0);
      glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
      glColor4f (0.9, 0.9, 0.9, 0.75);
      glRecti (0, 0, MARGIN_LEFT + m_width + MARGIN_RIGHT, (MARGIN_TOP + n * m_height + MARGIN_BOTTOM));
      glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth (2.0);
      glColor4f (0.8, 0.8, 0.8, 0.75);
      glRecti (0, 0, MARGIN_LEFT + m_width + MARGIN_RIGHT, (MARGIN_TOP + n * m_height + MARGIN_BOTTOM));
      glColor4f (0.0, 0.0, 0.0, 1.0);
      glTranslatef (MARGIN_LEFT, (MARGIN_TOP + m_height), 0.0);
      for (int i = 0; i < n; ++i)
        Text::Write (0, (i * m_height), m_items [i]);
      // Restore previous state.
      glPolygonMode (GL_FRONT, backup_glPolygonMode [0]);
      glPolygonMode (GL_BACK,  backup_glPolygonMode [1]);
      glLineWidth (backup_glLineWidth);
      (backup_glCullFace ? glEnable : glDisable) (GL_CULL_FACE);
    }

  } // namespace GL
} // namespace Aa

#endif // __AA_GLUT_MENU__

