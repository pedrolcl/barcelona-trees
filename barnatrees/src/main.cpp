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

#include <QArchive>

#include "gendermodel.h"
#include "plantmodel.h"
#include "plantproxymodel.h"
#include "speciesmodel.h"
#include "splashwindow.h"
#include "streetlistmodel.h"
#include "summarymodel.h"
#include "webdownloader.h"

using QArchive::DiskExtractor;

enum ConfigState { NormalState, EmptyState, NewState, OldState };

QDir g_configDir;
QString g_remoteTimestamp;
QString g_databaseFile;

QDateTime remoteTimestamp()
{
    const QUrl texturl("https://pedrolcl.github.io/barcelona-trees/barnatrees.txt");
    QEventLoop loop;
    WebDownloader dwnloader;

    QObject::connect(&dwnloader, &WebDownloader::downloadTextReady, &loop, [&](QString text) {
        g_remoteTimestamp = text;
        g_remoteTimestamp.remove(QChar::LineFeed);
        loop.exit();
    });
    QObject::connect(&dwnloader, &WebDownloader::downloadError, &loop, [&](QString msg) {
        qWarning() << Q_FUNC_INFO << "error:" << msg;
        loop.exit(-1);
    });
    dwnloader.downloadText(texturl);
    auto rc = loop.exec();
    if (rc == 0) {
        qDebug() << Q_FUNC_INFO << g_remoteTimestamp;
        return QDateTime::fromString(g_remoteTimestamp, Qt::ISODate);
    }
    return QDateTime();
}

QDateTime currentTimestamp()
{
    QFileInfo tsFileInfo(g_configDir, "barnatrees.txt");
    QFile tsFile(tsFileInfo.absoluteFilePath());
    if (!tsFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qFatal() << Q_FUNC_INFO << "Error opening local file" << tsFileInfo.fileName();
    }
    QString currentTimestamp = tsFile.readAll();
    currentTimestamp.remove(QChar::LineFeed);
    tsFile.close();
    qDebug() << Q_FUNC_INFO << currentTimestamp;
    return QDateTime::fromString(currentTimestamp, Qt::ISODate);
}

ConfigState analyzeState()
{
    // Check empty
    QFileInfo dbFileInfo(g_configDir, "barnatrees.db");
    QFileInfo tsFileInfo(g_configDir, "barnatrees.txt");
    if (!dbFileInfo.exists() || !tsFileInfo.exists()) {
        return ConfigState::EmptyState;
    }
    // Check new
    QFileInfo dbFileInfoNew(g_configDir, "barnatrees.db.7z");
    QFileInfo tsFileInfoNew(g_configDir, "barnatrees.txt.new");
    if (dbFileInfoNew.exists() && tsFileInfoNew.exists()) {
        return ConfigState::NewState;
    }
    // check OldState
    QDateTime current_ts = currentTimestamp();
    QDateTime new_ts = remoteTimestamp();
    if ((current_ts.addDays(30) < QDateTime::currentDateTimeUtc())
        && (new_ts.isValid() && current_ts < new_ts)) {
        return ConfigState::OldState;
    }
    return ConfigState::NormalState;
}

void processEmptyState()
{
    QEventLoop loop;

    // Timestamp file

    QFile embeddedFile(":/barnatrees.txt");
    bool ok = embeddedFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!ok) {
        qFatal() << Q_FUNC_INFO << "Error opening embedded resource" << embeddedFile.fileName();
    }
    QString embedded_timestamp = embeddedFile.readAll();
    embedded_timestamp.remove(QChar::LineFeed);
    qDebug() << Q_FUNC_INFO << "embedded timestamp:" << embedded_timestamp;
    embeddedFile.close();

    QFileInfo tsFileInfo(g_configDir, "barnatrees.txt");
    qDebug() << Q_FUNC_INFO << tsFileInfo.fileName() << "exists:" << tsFileInfo.exists();
    if (tsFileInfo.exists()) {
        QFile::remove(tsFileInfo.absoluteFilePath());
    }
    qDebug() << Q_FUNC_INFO << "writting" << tsFileInfo.fileName();
    QFile tsfile(tsFileInfo.absoluteFilePath());
    ok = tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!ok) {
        qFatal() << Q_FUNC_INFO << "Error opening output file" << tsfile.fileName();
    }
    tsfile.write(embedded_timestamp.toUtf8());
    tsfile.close();
    qDebug() << Q_FUNC_INFO << tsFileInfo.fileName() << "created";

    // Database file

    QFile orig(":/barnatrees.db.7z");
    if (!orig.exists()) {
        qCritical() << Q_FUNC_INFO << "barnatrees.db.7z resource is missing! aborting";
    }
    ok = orig.open(QIODevice::ReadOnly);
    if (!ok) {
        qFatal() << Q_FUNC_INFO << "Error opening compressed resource" << orig.fileName();
    }
    DiskExtractor extractor(&orig);
    extractor.setOutputDirectory(g_configDir.absolutePath());

    QObject::connect(&extractor, &DiskExtractor::finished, [&]() {
        qInfo() << Q_FUNC_INFO << "extracted File(s) Successfully!";
        orig.close();
        loop.exit();
    });
    QObject::connect(&extractor, &DiskExtractor::error, [&](short code) {
        qInfo() << Q_FUNC_INFO << "an error has occurred:" << QArchive::errorCodeToString(code);
        orig.close();
        loop.exit(-1);
    });
    extractor.start();
    loop.exec();
}

void processOldState()
{
    const QUrl dataurl("https://pedrolcl.github.io/barcelona-trees/barnatrees.db.7z");
    WebDownloader dwnloader;
    QEventLoop loop;
    qDebug() << Q_FUNC_INFO;

    // Timestamp file

    QFile tsfile(g_configDir.absoluteFilePath("barnatrees.txt.new"));
    bool ok = tsfile.open(QIODevice::WriteOnly | QIODevice::Text);
    if (ok) {
        tsfile.write(g_remoteTimestamp.toUtf8());
        tsfile.close();
    } else {
        qWarning() << Q_FUNC_INFO << "error opening output file" << tsfile.fileName();
    }

    // Compressed Database File

    QObject::connect(&dwnloader,
                     &WebDownloader::downloadProgress,
                     &loop,
                     [&](qint64 received, qint64 total) {
                         qDebug() << "download progress: %" << (1.0 * received / total) * 100.0;
                     });
    QObject::connect(&dwnloader, &WebDownloader::downloadError, &loop, [&](QString msg) {
        qWarning() << Q_FUNC_INFO << "error: " << msg;
        loop.exit(-1);
    });
    QObject::connect(&dwnloader, &WebDownloader::downloadSuccessful, &loop, [&]() {
        qDebug() << Q_FUNC_INFO << "finished";
        loop.exit();
    });
    QFileInfo finfo(g_configDir, "barnatrees.db.7z");
    dwnloader.downloadBinFile(dataurl, finfo.absoluteFilePath());

    loop.exec();
}

void processNewState()
{
    QEventLoop loop;
    QFileInfo cmpfileinfo(g_configDir, "barnatrees.db.7z");
    qDebug() << Q_FUNC_INFO << cmpfileinfo.fileName() << "exists:" << cmpfileinfo.exists();

    if (cmpfileinfo.exists()) {
        if (QFile::exists(g_configDir.absoluteFilePath("barnatrees.db"))) {
            qDebug() << Q_FUNC_INFO << "removing old barnatrees.db";
            QFile::remove(g_configDir.absoluteFilePath("barnatrees.db"));
        }
        QFile cmpfile(cmpfileinfo.absoluteFilePath());
        bool ok = cmpfile.open(QIODevice::ReadOnly);
        if (!ok) {
            qFatal() << Q_FUNC_INFO << "Error opening compressed database" << cmpfile.fileName();
        }

        DiskExtractor extractor(&cmpfile);
        extractor.setOutputDirectory(g_configDir.absolutePath());

        QObject::connect(&extractor, &DiskExtractor::finished, [&]() {
            qInfo() << "Extracted File(s) Successfully!";
            cmpfile.close();
            cmpfile.remove();
            qDebug() << Q_FUNC_INFO << "new remote barnatrees.db";
            loop.exit();
        });
        QObject::connect(&extractor, &DiskExtractor::error, [&](short code) {
            qInfo() << "An error has occurred:" << QArchive::errorCodeToString(code);
            cmpfile.close();
            cmpfile.remove();
            loop.exit(-1);
        });
        extractor.start();

        loop.exec();

        if (QFile::exists(g_configDir.absoluteFilePath("barnatrees.txt"))) {
            qDebug() << Q_FUNC_INFO << "removing old barnatrees.txt";
            QFile::remove(g_configDir.absoluteFilePath("barnatrees.txt"));
        }
        if (QFile::exists(g_configDir.absoluteFilePath("barnatrees.txt.new"))) {
            QFile::rename(g_configDir.absoluteFilePath("barnatrees.txt.new"),
                          g_configDir.absoluteFilePath("barnatrees.txt"));
            qDebug() << Q_FUNC_INFO << "new remote barnatrees.txt";
        }
    }
}

void processConfig(bool &dbFileNew)
{
    ConfigState state = analyzeState();
    switch (state) {
    case ConfigState::NormalState:
        qDebug() << Q_FUNC_INFO << "normal state";
        break;
    case ConfigState::EmptyState:
        qDebug() << Q_FUNC_INFO << "empty state";
        processEmptyState();
        break;
    case ConfigState::OldState:
        qDebug() << Q_FUNC_INFO << "old state";
        processOldState();
        dbFileNew = true;
        break;
    case ConfigState::NewState:
        qDebug() << Q_FUNC_INFO << "new state";
        processNewState();
        break;
    }
}

int main(int argc, char **argv)
{
    QGuiApplication::setApplicationName("barnatrees");
    QGuiApplication::setOrganizationName("BarcelonaTrees");
    QGuiApplication::setApplicationVersion(QT_STRINGIFY(APPVER));
    QGuiApplication::setDesktopFileName("barnatrees");
    QGuiApplication app(argc,argv);
    app.setWindowIcon(QIcon(":/barnatrees_icon64.png"));
    QIcon::setThemeName("example");

    qDebug() << Q_FUNC_INFO << "version:" << QT_STRINGIFY(APPVER) << QT_STRINGIFY(GITVER);
    qDebug() << Q_FUNC_INFO
             << "SSL version for build: " << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << Q_FUNC_INFO
             << "SSL version for run-time: " << QSslSocket::sslLibraryVersionString();

#if !defined(Q_OS_ANDROID)
    SplashWindow splash;
    splash.setMessage("Barcelona Trees v" QT_STRINGIFY(APPVER));
    splash.show();
    QCoreApplication::processEvents();
    qDebug() << "Barcelona Trees v" << QT_STRINGIFY(APPVER);
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
    qDebug() << "locale:" << locale << configuredLanguage;
    if ((locale.language() != QLocale::C) && (locale.language() != QLocale::English)) {
        if (trq.load(locale, QLatin1String("qt"), QLatin1String("_"), QLatin1String(":/"))) {
            QCoreApplication::installTranslator(&trq);
        } else {
            qWarning() << Q_FUNC_INFO << "Failure loading Qt translations for"
                       << configuredLanguage;
        }
        if (trp.load(locale, QLatin1String("barnatrees"), QLatin1String("_"), QLatin1String(":/"))) {
            QCoreApplication::installTranslator(&trp);
        } else {
            qWarning() << Q_FUNC_INFO << "Failure loading program translations for"
                       << configuredLanguage;
        }
        QLocale::setDefault(locale);
    }

    bool newDatabase = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));

    g_configDir = QDir{QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation)};

    processConfig(newDatabase);

    QFileInfo dbFileInfo(g_configDir, "barnatrees.db");
    if (!dbFileInfo.exists()) {
        qCritical() << "database file is not available";
        return -1;
    }
    g_databaseFile = dbFileInfo.absoluteFilePath();
    qDebug() << Q_FUNC_INFO << "Database file path:" << g_databaseFile;

    db.setDatabaseName(g_databaseFile);
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
