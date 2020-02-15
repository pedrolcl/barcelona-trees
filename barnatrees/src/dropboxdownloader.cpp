/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

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
