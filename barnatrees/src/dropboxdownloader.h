#ifndef DROPBOXDOWNLOADER_H
#define DROPBOXDOWNLOADER_H

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
