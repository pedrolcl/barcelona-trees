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

class DownloadManager : public QObject
{
	Q_OBJECT

private:
	QNetworkAccessManager m_manager;
	QVector<QNetworkReply *> m_currentDownloads;
	QList<QUrl> m_pendingDownloads;

public:
	explicit DownloadManager(QObject *parent = nullptr);
	void doDownload(const QUrl &url);
	static QString saveFileName(const QUrl &url);
	bool saveToDisk(const QString &filename, QIODevice *data);
	static bool isHttpRedirect(QNetworkReply *reply);
	void addDownload(const QString urlString);

public slots:
	void execute();
	void downloadFinished(QNetworkReply *reply);
	void sslErrors(const QList<QSslError> &errors);

signals:
	void done();
	void fileReady(QString fileName);
};

#endif // DOWNLOADMANAGER_H
