#ifndef AA_QGLSL__H
#define AA_QGLSL__H

#include <QResource>

namespace Aa
{
  namespace QGLSL
  {

    inline
    std::string Resource (const QString & name)
      throw (Aa::FileNotFound)
    {
      QResource r (name);

      if (! r.isValid ())
        throw Aa::FileNotFound (name.toStdString ());

      QByteArray bytes ((const char *) r.data (), r.size ());
      return (r.isCompressed () ? qUncompress (bytes) : bytes).toStdString ();
    }

    inline
    void SetStrings (const std::map<const char *, const char *> & m)
    {
      for (auto i = m.begin (); i != m.end (); ++i)
        Aa::GLSL::Program::SetString (i->first, Resource (i->second));
    }

  } // namespace QGLSL
} //namespace Aa

#endif // AA_QGLSL__H

