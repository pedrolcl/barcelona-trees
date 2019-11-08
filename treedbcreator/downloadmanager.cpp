#include <QFileInfo>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QDateTime>
#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent)
{
    m_connection = connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
                           SLOT(onManagerFinished(QNetworkReply*)));
}

void DownloadManager::startDownloads()
{
    disconnect(m_connection);
    m_connection = connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
                           SLOT(downloadFinished(QNetworkReply*)));
    m_currentDownload = nullptr;
    if (m_pendingDownloads.isEmpty()) {
        emit done();
    } else {
        doDownload(m_pendingDownloads.takeFirst());
    }
}

void DownloadManager::doDownload(const OpenDataset& res)
{
    QUrl url = res.url;
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = m_manager.get(request);
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif
    m_currentDataset = res;
    m_currentDownload = reply;
}

QString DownloadManager::saveFileName(const QUrl &url) const
{
    QFileInfo pathInfo(url.path());
    QString basename = pathInfo.baseName();
    QString suffix = pathInfo.suffix();
    if (basename.isEmpty())
        basename = "download";
    if (suffix.isEmpty())
        suffix = "xml";
    QString completeBN = basename + '.' + suffix;
    if (QFile::exists(completeBN)) {
        // already exists, don't overwrite
        QString now = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmmss");
        QString completeNewName = basename + '_' + now + '.' + suffix;
        QFile::rename(completeBN, completeNewName);
    }
    return completeBN;
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

bool DownloadManager::isHttpRedirect(QNetworkReply *reply) const
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
            || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::execute()
{
    QNetworkRequest request(QUrl("https://opendata-ajuntament.barcelona.cat/data/api/3/action/package_search?fq=tags:Arbrat"));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *reply = m_manager.get(request);
#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif
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
            qDebug() << "Request was redirected but not followed.";
        } else {
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply)) {
                qDebug() << "Download of" << url << "succeeded, saved to:" << filename;
                m_currentDataset.fileName = filename;
                emit downloadReady(m_currentDataset);
            }
        }
    }

    reply->deleteLater();
    m_currentDownload = nullptr;

    if (m_pendingDownloads.isEmpty()) {
        emit done();
    } else {
        doDownload(m_pendingDownloads.takeFirst());
    }
}

void DownloadManager::onManagerFinished(QNetworkReply *reply)
{
    QString dsName;
    QDateTime dsTime;
    QUrl dsUri;
    auto statusCode = reply->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    if (reply->isFinished() && statusCode.toInt() == 200) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (doc.isObject()) {
            QJsonObject obj = doc.object();
            if (obj.contains("success") && obj["success"].isBool()) {
                auto success = obj["success"].toBool();
                if (success && obj.contains("result")) {
                    QJsonObject result = obj["result"].toObject();
                    int resCount = 0;
                    if (result.contains("count") && result["count"].isDouble()) {
                        resCount = result["count"].toInt();
                    }
                    if (result.contains("results") && result["results"].isArray()) {
                        QJsonArray results = result["results"].toArray();
                        if (!results.isEmpty() && (resCount == results.count())) {
                            //qDebug() << "results:" << resCount << results.count() << results.size();
                            for(int i=0; i<resCount; ++i) {
                                QJsonObject dataset = results[i].toObject();
                                if (dataset.contains("resources") && dataset["resources"].isArray()) {
                                    QJsonArray resources = dataset["resources"].toArray();
                                    //qDebug() << "resources:" << resources.count() << resources.size();
                                    for(int j=0; j<resources.size(); ++j) {
                                        QJsonObject resource = resources[j].toObject();
                                        if (resource.contains("format") && resource["format"].isString()) {
                                            QString format = resource["format"].toString();
                                            if (format == "XML") {
                                                QString sqa = resource["qa"].toString();
                                                dsTime = QDateTime::fromString(sqa.mid(13, 26), Qt::ISODate);
                                                dsName = resource["name"].toString();
                                                dsUri = QUrl(resource["url"].toString());
                                                //qDebug() << "------------- resource:" << j;
                                                //qDebug() << "updated:" << dsTime;
                                                //qDebug() << "name:" << dsName;
                                                //qDebug() << "url:" << dsUri;
                                                m_pendingDownloads.append(OpenDataset{ dsName, QString(), dsUri, dsTime, 0, 0 });
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    startDownloads();
}
