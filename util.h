#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QVariant>
#include <QByteArray>

// STD includes


namespace util {

  QString toString (const QVariant& v) {
    return v.toString();
  }
    
  QString toString (const QByteArray& b) {
    return b.toUtf8();
  }
  
  QString toString (const int i) {
    return QVariant(i).toString();
  }
  
  QString toString (const float f) {
    return QVariant(f).toString();
  }
  
  QString toString (const double d) {
    return QVariant(d).toString();
  }
  
  QString toString (const bool b) {
    return QVariant(b).toString();
  }
  
  

}





#endif // UTIL_H
