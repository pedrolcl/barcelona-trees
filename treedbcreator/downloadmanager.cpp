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
    m_connection = connect(&m_manager, &QNetworkAccessManager::finished,
                           this, &DownloadManager::onManagerFinished);
}

void DownloadManager::startDownloads()
{
    disconnect(m_connection);
    m_connection = connect(&m_manager, &QNetworkAccessManager::finished,
                           this, &DownloadManager::downloadFinished);
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif
    QNetworkReply *reply = m_manager.get(request);
#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors,
            this, &DownloadManager::sslErrors);
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
        suffix = "json";
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
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
#endif
    QNetworkReply *reply = m_manager.get(request);
#if QT_CONFIG(ssl)
    connect(reply, &QNetworkReply::sslErrors,
            this, &DownloadManager::sslErrors);
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
                                if (dataset.contains("resources") && dataset["resources"].isArray() &&
                                    dataset.contains("code") && dataset["code"].isString()) {
                                    QJsonArray resources = dataset["resources"].toArray();
                                    QString code = dataset["code"].toString();
                                    //qDebug() << code << "resources:" << resources.count() << resources.size();
                                    if (code == "ARBRES_INTERES_LOCAL") {
                                        //qDebug() << "ignored";
                                        continue;
                                    }
                                    QString lastName;
                                    QUrl lastUri;
                                    QDateTime lastTime = QDateTime::fromMSecsSinceEpoch(0);
                                    for(int j=0; j<resources.size(); ++j) {
                                        QJsonObject resource = resources[j].toObject();
                                        if (resource.contains("format") && resource["format"].isString()) {
                                            QString format = resource["format"].toString();
                                            if (format == "JSON") {
                                                QDateTime dsTime = QDateTime::fromString(resource["created"].toString(), Qt::ISODate);
                                                QString dsName = resource["name"].toString();
                                                QUrl dsUri = QUrl(resource["url"].toString());
                                                //qDebug() << "------------- resource:" << j;
                                                //qDebug() << "created:" << dsTime;
                                                //qDebug() << "name:" << dsName;
                                                //qDebug() << "url:" << dsUri;
                                                if (dsTime > lastTime) {
                                                    //qDebug() << "latest";
                                                    lastTime = dsTime;
                                                    lastName = dsName;
                                                    lastUri = dsUri;
                                                }
                                            }
                                        }
                                    }
                                    if (!lastName.isEmpty() && lastUri.isValid()) {
                                        m_pendingDownloads.append(OpenDataset{ lastName, QString(), lastUri, lastTime, 0, 0 });
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
