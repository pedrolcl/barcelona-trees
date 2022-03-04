#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H
/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include <QString>
#include <QList>
#include <QVector>
#include <QIODevice>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSslError>
#include <QUrl>

struct OpenDataset {
    QString name;
    QString fileName;
    QUrl url;
    QDateTime time;
    int species;
    int specimens;
};

class DownloadManager : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager m_manager;
    QMetaObject::Connection m_connection;
    QNetworkReply *m_currentDownload;
    OpenDataset m_currentDataset;
    QList<OpenDataset> m_pendingDownloads;

public:
    explicit DownloadManager(QObject *parent = nullptr);
    void startDownloads();
    void doDownload(const OpenDataset &res);
    QString saveFileName(const QUrl &url) const;
    bool isHttpRedirect(QNetworkReply *reply) const;
    bool saveToDisk(const QString &filename, QIODevice *data);

public slots:
    void execute();
    void downloadFinished(QNetworkReply *reply);
    void onManagerFinished(QNetworkReply *reply);
    void sslErrors(const QList<QSslError> &errors);

signals:
    void done();
    void downloadReady(OpenDataset ds);
};

#endif // DOWNLOADMANAGER_H
