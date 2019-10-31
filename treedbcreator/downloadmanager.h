#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

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
