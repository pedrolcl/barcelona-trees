/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2026 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include <QDir>
#include <QFileInfo>
#include <QGeoCoordinate>
#include <QGuiApplication>
#include <QIcon>
#include <QLibraryInfo>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QQuickView>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSslSocket>
#include <QStandardPaths>
#include <QTimer>
#include <QTranslator>
#include "gendermodel.h"
#include "plantmodel.h"
#include "plantproxymodel.h"
#include "q7z_extract.h"
#include "q7z_facade.h"
#include "speciesmodel.h"
#include "splashwindow.h"
#include "streetlistmodel.h"
#include "summarymodel.h"
#include "webdownloader.h"

int downloadFromWeb(QDir &destDir, QString current, bool &dbFileNew)
{
    const QUrl texturl("https://pedrolcl.github.io/barcelona-trees/barnatrees.txt");
    const QUrl dataurl("https://pedrolcl.github.io/barcelona-trees/barnatrees.db.7z");
    QEventLoop loop;

    QString newtimestamp;
    WebDownloader dwnloader;
    QObject::connect(&dwnloader,
                     &WebDownloader::downloadProgress,
                     &loop,
                     [&](qint64 received, qint64 total) {
                         qDebug() << "%" << (1.0 * received / total) * 100.0;
                     });
    QObject::connect(&dwnloader, &WebDownloader::downloadError, &loop, [&](QString msg) {
        qWarning() << "download error: " << msg;
        loop.exit(-1);
    });
    QObject::connect(&dwnloader, &WebDownloader::downloadSuccessful, &loop, [&]() {
        qDebug() << "download finished";
        QFile tsfile(destDir.absoluteFilePath("barnatrees.txt.remote"));
        bool ok = tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
        if (ok) {
            tsfile.write(newtimestamp.toUtf8());
            tsfile.close();
        } else {
            qWarning() << "Error opening output file" << tsfile.fileName();
        }
        loop.exit();
    });
    QObject::connect(&dwnloader, &WebDownloader::downloadTextReady, &loop, [&](QString text) {
        qDebug() << "text download finished:" << text;
        newtimestamp = text;
    });
    QObject::connect(&dwnloader, &WebDownloader::readyForNext, &loop, [&]() {
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
    auto rc = loop.exec();
    return rc;
}

QString localDatabaseFile(bool& dbFileNew)
{
    bool ok{false};
    QString currenttimestamp;

    QFile embeddedFile(":/barnatrees.txt");
    ok = embeddedFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!ok) {
        qFatal() << "Error opening embedded resource" << embeddedFile.fileName();
    }
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
            ok = cmpfile.open(QIODevice::ReadOnly);
            if (!ok) {
                qFatal() << "Error opening compressed database" << cmpfile.fileName();
            }
            Q7z::extractArchive(&cmpfile, destDir.absolutePath());
            cmpfile.close();
            cmpfile.remove();
        } catch (const Q7z::SevenZipException &e) {
            qWarning() << e.message();
            cmpfile.close();
            QFile::remove(cmpfileinfo.absoluteFilePath());
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
        ok = tsFile.open(QIODevice::ReadOnly | QIODevice::Text);
        if (!ok) {
            qFatal() << "Error opening local file" << tsFileInfo.fileName();
        }
        currenttimestamp = tsFile.readAll();
        tsFile.close();

        currenttimestamp.remove(QChar::LineFeed);

        if (!cmpfileinfo.exists()) {
            QDateTime ts = QDateTime::fromString(currenttimestamp, Qt::ISODate);
            if (ts.addDays(30) < QDateTime::currentDateTimeUtc()) {
                qWarning() << "The database is old. Updating from the cloud.";
                downloadFromWeb(destDir, currenttimestamp, dbFileNew);
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
            ok = orig.open(QIODevice::ReadOnly);
            if (!ok) {
                qFatal() << "Error opening compressed resource" << orig.fileName();
            }
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
        ok = tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
        if (!ok) {
            qFatal() << "Error opening compressed resource" << tsfile.fileName();
        }
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
    //QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setApplicationVersion(QT_STRINGIFY(APPVER));
    QGuiApplication app(argc,argv);
    app.setWindowIcon(QIcon(":/barnatrees_icon64.png"));
    QIcon::setThemeName("example");

    qDebug()<<"version:" << QT_STRINGIFY(APPVER) << QT_STRINGIFY(GITVER);
    //qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    //qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionString();

#if !defined(Q_OS_ANDROID)
    SplashWindow splash;
    splash.setMessage("Barcelona Trees v" QT_STRINGIFY(APPVER));
    splash.show();
    QCoreApplication::processEvents();
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
    if ((locale.language() != QLocale::C) && (locale.language() != QLocale::English)) {
        if (trq.load(locale, QLatin1String("qt"), QLatin1String("_"), QLatin1String(":/"))) {
            QCoreApplication::installTranslator(&trq);
        } else {
            qWarning() << "Failure loading Qt translations for" << configuredLanguage;
        }
        if (trp.load(locale, QLatin1String("barnatrees"), QLatin1String("_"), QLatin1String(":/"))) {
            QCoreApplication::installTranslator(&trp);
        } else {
            qWarning() << "Failure loading program translations for" << configuredLanguage;
        }
        QLocale::setDefault(locale);
    }

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
    PlantModel plantModel;
    //qDebug() << "plants.columns:" << plantModel.columnCount();
    PlantProxyModel plantProxy(&plantModel);

    SummaryModel summaryModel;
    GenderModel genderModel;
    //qDebug() << "gender.columns:" << genderModel.columnCount();
    StreetListModel streetModel;
    FilterProxyModel streetFilter;
    streetFilter.setSourceModel(&streetModel);
    streetFilter.setFilterRegularExpression(QStringLiteral("^[\\*]*$"));
    //qDebug() << "filtered.rows:" << streetFilter.rowCount();

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
                                 QLatin1String("Imagine"), QLatin1String("Material"),
                                 QLatin1String("Universal") };
#if defined(Q_OS_MACOS)
    builtInStyles << QLatin1String("macOS");
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_IOS)
    builtInStyles << QLatin1String("iOS");
#elif defined(Q_OS_WINDOWS)
    builtInStyles << QLatin1String("Windows");
#endif

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperties({
        { "gitversion", QT_STRINGIFY(GITVER) },
        { "qtversion", QT_VERSION_STR },
        { "newDatabase", newDatabase },
        { "availableStyles", builtInStyles },
        { "speciesModel", QVariant::fromValue(&speciesModel) },
        { "plantModel", QVariant::fromValue(&plantModel) },
        { "plantProxy", QVariant::fromValue(&plantProxy) },
        { "summaryModel", QVariant::fromValue(&summaryModel) },
        { "genderModel", QVariant::fromValue(&genderModel) },
        { "streetModel", QVariant::fromValue(&streetFilter) }
    });
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    QObject::connect(&engine, &QQmlApplicationEngine::exit, &app, &QCoreApplication::quit);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QCoreApplication::quit);
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
#if !defined(Q_OS_ANDROID)
     else {
        QQuickWindow *qmlWindow = qobject_cast<QQuickWindow *>(engine.rootObjects().constFirst());
        QCoreApplication::processEvents();
        if (qmlWindow->isVisible()) {
            splash.close();
        } else {
            splash.finish(qmlWindow);
        }
    }
#endif
    return app.exec();
}
