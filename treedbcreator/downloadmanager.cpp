#include <QFileInfo>
#include <QDebug>
#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
	connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
			SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadManager::addDownload(const QString urlString)
{
	m_pendingDownloads.append(QUrl(urlString));
}

void DownloadManager::doDownload(const QUrl &url)
{
	QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
	QNetworkReply *reply = m_manager.get(request);

#if QT_CONFIG(ssl)
	connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
			SLOT(sslErrors(QList<QSslError>)));
#endif
	m_currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
	QString path = url.path();
	QString basename = QFileInfo(path).fileName();
	if (basename.isEmpty())
		basename = "download";
	if (QFile::exists(basename)) {
		// already exists, don't overwrite
		int i = 0;
		basename += '.';
		while (QFile::exists(basename + QString::number(i)))
			++i;
		basename += QString::number(i);
	}
	return basename;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Could not open" << filename << "for writing:" << file.errorString();
		return false;
	}
	file.write(data->readAll());
	file.close();
	return true;
}

bool DownloadManager::isHttpRedirect(QNetworkReply *reply)
{
	int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
	return statusCode == 301 || statusCode == 302 || statusCode == 303
		   || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::execute()
{
	if (m_pendingDownloads.isEmpty()) {
		qDebug() << "Qt Download example - downloads all URLs sequentially\n"
					"Usage: download url1 [url2... urlN]\n"
					"\n"
					"Downloads the URLs passed in the command-line to the local directory\n"
					"If the target file already exists, a .0, .1, .2, etc. is appended to\n"
					"differentiate.";
		emit done();
	} else {
		doDownload(m_pendingDownloads.takeFirst());
	}
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
	for (const QSslError &error : sslErrors)
		qDebug() << "SSL error:" << error.errorString();
#else
	Q_UNUSED(sslErrors)
#endif
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
	QUrl url = reply->url();
	if (reply->error()) {
		qDebug() << "Download of" << url << "failed: " << reply->errorString();
	} else {
		if (isHttpRedirect(reply)) {
            qDebug() << "Request was redirected.";
        } else {
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply)) {
                qDebug() << "Download of" << url << "succeeded, saved to:" << filename;
                emit fileReady(filename);
            }
        }
	}

	m_currentDownloads.removeAll(reply);
	reply->deleteLater();

	if (m_pendingDownloads.isEmpty()) {
		emit done();
	} else {
		doDownload(m_pendingDownloads.takeFirst());
	}
}
