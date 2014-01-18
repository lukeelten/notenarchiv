#ifndef UTIL_H
#define UTIL_H

#include <QString>
#include <QVariant>
#include <QByteArray>

namespace util {

  inline QString toString (const QVariant& v) {
    return v.toString();
  }

  inline QString toString (const QByteArray& b) {
    return QString(b);
  }

  inline QString toString (const int i) {
    return QVariant(i).toString();
  }

  inline QString toString (const float f) {
    return QVariant(f).toString();
  }

  inline QString toString (const double d) {
    return QVariant(d).toString();
  }

  inline QString toString (const bool b) {
    return QVariant(b).toString();
  }


  inline QByteArray toByteArray (const QString& str) {
      return str.toUtf8();
  }

  inline QByteArray toByteArray (const QVariant& v) {
      return v.toByteArray();
  }

}

#endif // UTIL_H
