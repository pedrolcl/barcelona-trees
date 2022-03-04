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

#include <QGuiApplication>
#include <QQuickView>
#include <QSslSocket>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QGeoCoordinate>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QSettings>
#include <QQuickStyle>
#include <QTranslator>
#include <QLibraryInfo>
#include <QIcon>
#include <QTimer>
#include "plantmodel.h"
#include "plantproxymodel.h"
#include "speciesmodel.h"
#include "summarymodel.h"
#include "gendermodel.h"
#include "streetlistmodel.h"
#include "q7z_facade.h"
#include "q7z_extract.h"
#include "dropboxdownloader.h"
#include "splashwindow.h"

int downloadFromCloud(QDir &destDir, QString current, bool& dbFileNew)
{
    const QUrl texturl("https://www.dropbox.com/s/e485ypwbdy113v9/barnatrees.txt?dl=1");
    const QUrl dataurl("https://www.dropbox.com/s/7o9aa8744mwjx7i/barnatrees.db.7z?dl=1");
    QEventLoop loop;

    QString newtimestamp;
    DropboxDownloader dwnloader;
    QObject::connect(&dwnloader, &DropboxDownloader::downloadProgress, [&](qint64 received, qint64 total) {
        qDebug() << "%" << (1.0 * received / total) * 100.0;
    });
    QObject::connect(&dwnloader, &DropboxDownloader::downloadError, [&](QString msg) {
        qWarning() << "download error: " << msg;
        loop.exit(-1);
    });
    QObject::connect(&dwnloader, &DropboxDownloader::downloadSuccessful, [&]() {
        qDebug() << "download finished";
        QFile tsfile(destDir.absoluteFilePath("barnatrees.txt.remote"));
        tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
        tsfile.write(newtimestamp.toUtf8());
        tsfile.close();
        loop.exit();
    });
    QObject::connect(&dwnloader, &DropboxDownloader::downloadTextReady, [&](QString text) {
        qDebug() << "text download finished:" << text;
        newtimestamp = text;
    });
    QObject::connect(&dwnloader, &DropboxDownloader::readyForNext, [&]() {
        qDebug() << "Next operation is ready";
        if (!newtimestamp.isEmpty() && (newtimestamp > current)) {
            QFileInfo finfo(destDir, "barnatrees.db.7z");
            dwnloader.downloadBinFile(dataurl, finfo.absoluteFilePath());
            dbFileNew = true;
        } else {
            qDebug() << "Nothing newer from the cloud";
            dbFileNew = false;
            loop.exit();
        }
    });
    dwnloader.downloadText(texturl);
    qDebug() << "entering loop";
    auto rc = loop.exec();
    qDebug() << "exiting loop";
    return rc;
}

QString localDatabaseFile(bool& dbFileNew)
{
    QString currenttimestamp;

    QFile embeddedFile(":/barnatrees.txt");
    embeddedFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString embedded_timestamp = embeddedFile.readAll();
    embeddedFile.close();

    QDir destDir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    if (!destDir.exists()) {
        if (!destDir.mkpath(".")) {
            qWarning() << "Error creating the database path";
            return QString();
        }
    }

    QFileInfo cmpfileinfo(destDir, "barnatrees.db.7z");
    if (cmpfileinfo.exists()) {
        if (QFile::exists(destDir.absoluteFilePath("barnatrees.db"))) {
            QFile::remove(destDir.absoluteFilePath("barnatrees.db"));
        }
        QFile cmpfile(cmpfileinfo.absoluteFilePath());
        try {
            cmpfile.open(QIODevice::ReadOnly);
            Q7z::extractArchive(&cmpfile, destDir.absolutePath());
            cmpfile.close();
            cmpfile.remove();
        } catch (const Q7z::SevenZipException &e) {
            qWarning() << e.message();
            cmpfile.close();
        }
        if (QFile::exists(destDir.absoluteFilePath("barnatrees.txt"))) {
            QFile::remove(destDir.absoluteFilePath("barnatrees.txt"));
        }
        if (QFile::exists(destDir.absoluteFilePath("barnatrees.txt.remote"))) {
            QFile::rename(destDir.absoluteFilePath("barnatrees.txt.remote"),
                          destDir.absoluteFilePath("barnatrees.txt"));
        }
    }

    QFileInfo tsFileInfo(destDir, "barnatrees.txt");
    if (tsFileInfo.exists()) {
        QFile tsFile(tsFileInfo.absoluteFilePath());
        tsFile.open(QIODevice::ReadOnly | QIODevice::Text);
        currenttimestamp = tsFile.readAll();
        tsFile.close();

        if (!cmpfileinfo.exists()) {
            QDateTime ts = QDateTime::fromString(currenttimestamp+'Z', Qt::ISODate);
            if (ts.addDays(7) < QDateTime::currentDateTimeUtc()) {
                qWarning() << "The database is old. Updating from the cloud.";
                downloadFromCloud(destDir, currenttimestamp, dbFileNew);
            }
        }
    }

    QFileInfo dbFileInfo(destDir, "barnatrees.db");
    if (!dbFileInfo.exists() || currenttimestamp < embedded_timestamp) {
        QFile orig(":/barnatrees.db.7z");
        if (!orig.exists()) {
            qCritical() << "barnatrees.db.7z resource is missing! aborting";
            return QString();
        }
        if (dbFileInfo.exists()) {
            QFile::remove(dbFileInfo.absoluteFilePath());
        }
        try {
            orig.open(QIODevice::ReadOnly);
            Q7z::extractArchive(&orig, destDir.absolutePath());
            orig.close();
        }
        catch (const Q7z::SevenZipException& e)
        {
            qWarning() << "extract database:" << e.message();
            orig.close();
            return QString();
        }
        if(!QFile::exists(dbFileInfo.absoluteFilePath())) {
            qWarning() << "copy database: extraction failed";
            return QString();
        }
        if (!QFile::setPermissions(dbFileInfo.absoluteFilePath(), QFile::WriteOwner | QFile::ReadOwner)) {
            qWarning() << "database file setPermissions() failed!";
            return QString();
        }
        QFile tsfile(tsFileInfo.absoluteFilePath());
        tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
        tsfile.write(embedded_timestamp.toUtf8());
        tsfile.close();
    }
    qDebug() << "Database file:" << dbFileInfo.absoluteFilePath();
    return dbFileInfo.absoluteFilePath();
}

int main(int argc, char **argv)
{
    QGuiApplication::setApplicationName("barnatrees");
    QGuiApplication::setOrganizationName("BarcelonaTrees");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setApplicationVersion(QT_STRINGIFY(APPVER));
    QGuiApplication app(argc,argv);
    app.setWindowIcon(QIcon("qrc:/barnatrees_icon64.png"));
    QIcon::setThemeName("example");

    qDebug()<<"version:" << QT_STRINGIFY(APPVER) << QT_STRINGIFY(GITVER);
    //qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    //qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionString();

#if !defined(Q_OS_ANDROID)
    SplashWindow splash;
    splash.setMessage("Barcelona Trees v" QT_STRINGIFY(APPVER));
    splash.show();
    QTimer::singleShot(2000, &splash, SLOT(close()));
#endif

    QSettings settings;
    QString defStyle = QQuickStyle::name();
    QString style = settings.value("style", defStyle).toString();
    if (!style.isEmpty() && style != defStyle ) {
        QQuickStyle::setStyle(style);
        settings.setValue("style", style);
    }
    QString defLang = QLocale::system().name().left(2);
    QString configuredLanguage = settings.value("language", defLang).toString();
    QTranslator trq;
    QTranslator trp;
    QLocale locale(configuredLanguage);
    //qDebug() << "locale:" << locale;
    if (trq.load(locale, QLatin1String("qt"), QLatin1String("_"), QLatin1String(":/"))) {
        QCoreApplication::installTranslator(&trq);
    } else {
        qWarning() << "Failure loading Qt5 translations for" << configuredLanguage;
    }
    if (trp.load(locale, QLatin1String("barnatrees"), QLatin1String("_"), QLatin1String(":/"))) {
        QCoreApplication::installTranslator(&trp);
    } else {
        qWarning() << "Failure loading program translations for" << configuredLanguage;
    }
    QLocale::setDefault(locale);

    bool newDatabase = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));
    Q7z::initSevenZ();
    QString dbFile = localDatabaseFile(newDatabase);
    if (dbFile.isEmpty())
        return -1;
    db.setDatabaseName(dbFile);
    if (!db.open()) {
        qCritical() << db.lastError();
        return -1;
    }

    SpeciesModel speciesModel;
    //qDebug() << "species.columns:" << speciesModel.columnCount();
    QGeoCoordinate locationBarna = QGeoCoordinate( 41.403216, 2.186674 );
    PlantModel plantModel;
    plantModel.setCenter(locationBarna);
    //qDebug() << "plants.columns:" << plantModel.columnCount();
    PlantProxyModel plantProxy(&plantModel);

    SummaryModel summaryModel;
    GenderModel genderModel;
    //qDebug() << "gender.columns:" << genderModel.columnCount();
    StreetListModel streetModel;
    FilterProxyModel streetFilter;
    streetFilter.setSourceModel(&streetModel);
    streetFilter.setFilterRegExp(QStringLiteral("^[\\*]*$"));
    //qDebug() << "filtered.rows:" << streetFilter.rowCount();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gitversion", QT_STRINGIFY(GITVER));
    engine.rootContext()->setContextProperty("qtversion", QT_VERSION_STR);
    engine.rootContext()->setContextProperty("newDatabase", newDatabase);
    engine.rootContext()->setContextProperty("availableStyles", QQuickStyle::availableStyles());
    engine.rootContext()->setContextProperty("speciesModel", &speciesModel);
    engine.rootContext()->setContextProperty("plantModel", &plantModel);
    engine.rootContext()->setContextProperty("plantProxy", &plantProxy);
    engine.rootContext()->setContextProperty("summaryModel", &summaryModel);
    engine.rootContext()->setContextProperty("genderModel", &genderModel);
    engine.rootContext()->setContextProperty("streetModel", &streetFilter);
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    QObject::connect(&engine, &QQmlApplicationEngine::exit, &app, &QCoreApplication::quit);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QCoreApplication::quit);
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
