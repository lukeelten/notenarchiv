#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>
#include <QString>
#include <QUrl>
#include <QVariant>

#include "UpdateInformation.h"

UpdateInformation::UpdateInformation(const QString& json) {
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    m_json = doc.object();
}

QList<QUrl> UpdateInformation::GetFiles() const {
    QList<QUrl> list;

    if (!m_json["version"].isArray())
        return qMove(list);

    QVariantList a = m_json["files"].toArray().toVariantList();

    for (auto i = a.begin(); i != a.end(); i++) {
        QUrl url((*i).toString());

        list.append(qMove(url));
    }


    return qMove(list);
}
