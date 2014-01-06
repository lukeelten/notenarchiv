#ifndef UPDATEINFORMATION_H
#define UPDATEINFORMATION_H

#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QUrl>

class UpdateInformation
{
public:
    UpdateInformation();
    ~UpdateInformation() = default;

    int GetRemoteVersion() const { return m_json["version"].toInt(); }
    QList<QUrl> GetFiles() const;



private:
    QJsonObject m_json;

};

#endif // UPDATEINFORMATION_H
