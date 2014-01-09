#include <math.h>
#include <QDebug>
#include <QVariant>
#include <QMessageBox>

#include "Updater.h"
#include "ui_Updater.h"

const QString Updater::DEFAULT_URL = "http://dev.lukeelten.de/notenarchiv/notenarchiv.json";
const int PROGRESS_MAX = 100000;

Updater::Updater(const int local_version, const QString& url, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Updater),
    m_net(new QNetworkAccessManager(this))
{
    m_reply = nullptr;
    m_numfiles = 0;
    m_currentfile = 0;

    if (url.isEmpty())
        m_url = QUrl(Updater::DEFAULT_URL);
    else
        m_url = QUrl(url);

    ui->setupUi(this);

    m_lversion = local_version;
    ui->text->setText("Updateprogramm für Notenarchiv. Aktuelle Version: " + QVariant(m_lversion).toString());

    connect(ui->buttonSearch, SIGNAL(clicked()), this, SLOT(GetInformation()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), this, SLOT(DoUpdate()));

    ui->progress->setEnabled(false);
}

Updater::~Updater()
{
    if (m_net)
        delete m_net;

    if (ui)
        delete ui;
}

void Updater::GetInformation() {
    ui->text->setText("Suche nach Aktualisierungen ...");
    ui->progress->setEnabled(true);
    ui->progress->setValue(0);

    m_reply = m_net->get(QNetworkRequest(m_url));
    m_numfiles = 1;
    m_currentfile = 0;

    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(Progress(qint64,qint64)));
    connect(m_reply, SIGNAL(finished()), this, SLOT(FinishedSearch()));


}

void Updater::DoUpdate() {
    if (!m_info.IsValid())
        GetInformation();

}

void Updater::Progress(qint64 received, qint64 total) {
    if (m_numfiles == 1) {
        ui->progress->setMaximum(total);
        ui->progress->setValue(received);

        return;
    }

    ui->progress->setMaximum(PROGRESS_MAX);

    int cur = 0;
    if (m_currentfile > 0)
        cur = (PROGRESS_MAX / m_numfiles) * m_currentfile;

    double progress = (PROGRESS_MAX / m_numfiles) * (received / total);

    cur += round(progress);

    ui->progress->setValue(cur);
}

void Updater::FinishedSearch() {
    ui->text->setText("Finished");

    //QMessageBox::information(this, "Info", str, QMessageBox::Close, QMessageBox::Close);

    m_info.SetJson(m_reply->readAll());

    ui->text->setText("Remote Version: " + QVariant(m_info.GetRemoteVersion()).toString());
}
