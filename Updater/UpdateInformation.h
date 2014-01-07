#ifndef UPDATEINFORMATION_H
#define UPDATEINFORMATION_H

#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QString>
#include <QUrl>

class UpdateInformation
{

public:
    UpdateInformation(const QString& json = "");
    ~UpdateInformation() = default;

    // Kopieren
    UpdateInformation(const UpdateInformation& ) = default;
    UpdateInformation& operator = (const UpdateInformation& ) = default;

    // Move
    UpdateInformation(UpdateInformation&& ) = default;
    UpdateInformation& operator = (UpdateInformation&& ) = default;


    int GetRemoteVersion() const { return m_json["version"].toInt(); }
    QList<QUrl> GetFiles() const;

    //bool ShouldUpdate(const int local_version) const { return (GetRemoteVersion() > local_version); }
    bool IsValid () const { return (!m_json.isEmpty()); }

    void SetJson(const QString& json);

private:
    QJsonObject m_json;

};

#endif // UPDATEINFORMATION_H
