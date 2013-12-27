#ifndef FILE_H
#define FILE_H

#include <QString>

class File
{
public:
    File();


private:
    int m_id;
    QString m_instrument, m_part, m_file;

    bool* m_changed;
};

#endif // FILE_H
