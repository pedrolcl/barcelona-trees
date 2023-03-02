#ifndef DROPBOXDOWNLOADER_H
#define DROPBOXDOWNLOADER_H

/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#include <QObject>
#include <QNetworkAccessManager>
#include <QFile>
#include <QNetworkReply>
#include <QPointer>

class DropboxDownloader : public QObject
{
    Q_OBJECT
public:
    explicit DropboxDownloader(QObject* parent= nullptr);
    ~DropboxDownloader();

    void downloadText(QUrl url);
    void downloadBinFile(QUrl url, QString fileName);
    void cancelDownload();

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadSuccessful();
    void downloadError(QString errorString);
    void downloadTextReady(QString text);
    void readyForNext();

private slots:
    void readData();
    void finishDownload();

private:
    void abortDownloadPrivate();

    QNetworkAccessManager m_nam;
    QUrl m_downloadUrl;
    QFile m_destinationFile;
    QPointer<QNetworkReply> m_networkReply;
};

#endif // DROPBOXDOWNLOADER_H
