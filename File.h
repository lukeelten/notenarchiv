#ifndef FILE_H
#define FILE_H

#include <QString>
#include <QSqlRecord>

class File
{

public:

    File(const File& ) = default;
    File& operator = (const File& ) = default;

    File(File&& ) = default;
    File& operator = (File&& ) = default;

};

#endif // FILE_H
