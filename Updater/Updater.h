#ifndef UPDATER_H
#define UPDATER_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QUrl>

#include "UpdateInformation.h"

namespace Ui {
class Updater;
}

class Updater : public QDialog
{
    Q_OBJECT

public:
    explicit Updater(const int local_version, const QString& url = "", QWidget *parent = 0);
    ~Updater();

public slots:
    void GetInformation();
    void DoUpdate();

    void Progress(qint64 received, qint64 total);
    void FinishedSearch();

private:
    Ui::Updater *ui;

    QNetworkAccessManager* m_net;
    QNetworkReply* m_reply;
    UpdateInformation m_info;

    int m_lversion, m_numfiles, m_currentfile;
    QUrl m_url;

    static const QString DEFAULT_URL;
};

#endif // UPDATER_H
