#include <QDebug>
#include "dropboxdownloader.h"

DropboxDownloader::DropboxDownloader(QObject* parent) :QObject(parent)
{ }

DropboxDownloader::~DropboxDownloader()
{ }

void DropboxDownloader::downloadText(QUrl url)
{
    if (m_networkReply) {
        qWarning() << "Error: another download is running";
        return;
    }
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_networkReply = m_nam.get(request);
    connect(m_networkReply, &QObject::destroyed, this, &DropboxDownloader::readyForNext);
    connect(m_networkReply, &QNetworkReply::finished, [&]() {
        if (m_networkReply->error() == QNetworkReply::NoError) {
            int status = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
            if (status == 200) {
                QString txt = m_networkReply->readAll();
                emit downloadTextReady(txt);
            }
            m_networkReply->deleteLater();
        } else {
            abortDownloadPrivate();
            emit downloadError(m_networkReply->errorString());
        }
    });
}

void DropboxDownloader::downloadBinFile(QUrl url, QString fileName)
{
    if (m_networkReply) {
        qWarning() << "Error: another download is running";
        return;
    }
    m_destinationFile.setFileName(fileName);
    if (!m_destinationFile.open(QIODevice::WriteOnly)) {
        emit downloadError(m_destinationFile.errorString());
        return;
    }
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_networkReply = m_nam.get(request);
    connect(m_networkReply, &QIODevice::readyRead, this, &DropboxDownloader::readData);
    connect(m_networkReply, &QNetworkReply::downloadProgress, this, &DropboxDownloader::downloadProgress);
    connect(m_networkReply, &QNetworkReply::finished, this, &DropboxDownloader::finishDownload);
    //connect(m_networkReply, &QObject::destroyed, this, &DropboxDownloader::readyForNext);
}

void DropboxDownloader::cancelDownload()
{
    if (m_networkReply) {
        abortDownloadPrivate();
    }
}

void DropboxDownloader::readData()
{
    QByteArray data = m_networkReply->readAll();
    m_destinationFile.write(data);
}

void DropboxDownloader::finishDownload()
{
    if (m_networkReply->error() == QNetworkReply::NoError) {
        int status = m_networkReply->attribute( QNetworkRequest::HttpStatusCodeAttribute ).toInt();
        if (status == 200) {
            QByteArray data = m_networkReply->readAll();
            m_destinationFile.write(data);
            m_destinationFile.close();
            emit downloadSuccessful();
        }
        m_networkReply->deleteLater();
    } else {
        abortDownloadPrivate();
        emit downloadError(m_networkReply->errorString());
    }
}

void DropboxDownloader::abortDownloadPrivate()
{
    m_networkReply->abort();
    m_networkReply->deleteLater();
    m_destinationFile.close();
    m_destinationFile.remove();
}
