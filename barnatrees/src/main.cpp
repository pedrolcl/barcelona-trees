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
#include "plantmodel.h"
#include "speciesmodel.h"
#include "summarymodel.h"

#define _STR(x) #x
#define STRINGIFY(x) _STR(x)

QString localDatabaseFile()
{
    QFile c1File(":/barnatrees.txt");
    c1File.open(QIODevice::ReadOnly | QIODevice::Text);
    QString c1txt = c1File.readAll();
    c1File.close();
    //QDateTime d1 = QDateTime::fromString(c1txt+'Z', Qt::ISODate);

    QDir destDir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    if (!destDir.exists()) {
        if (!destDir.mkpath(".")) {
            qWarning() << "Error creating the database path";
            return QString();
        }
    }

    QString c2txt;
    QFileInfo c2(destDir, "barnatrees.txt");
    if (c2.exists()) {
        QFile c2file(c2.absoluteFilePath());
        c2file.open(QIODevice::ReadOnly | QIODevice::Text);
        c2txt = c2file.readAll();
        c2file.close();

        QDateTime d2 = QDateTime::fromString(c2txt+'Z', Qt::ISODate);
        if (d2.addDays(14) < QDateTime::currentDateTimeUtc()) {
            qWarning() << "The database is old. An update is advisable.";
        }
    }

    QFileInfo destFile(destDir, "barnatrees.db");
    if (!destFile.exists() || c2txt < c1txt) {
        QFile orig(":/barnatrees.db");
        if (!orig.exists()) {
            qCritical() << "barnatrees.db resource is missing! aborting";
            return QString();
        }
        if (destFile.exists()) {
            QFile::remove(destFile.absoluteFilePath());
        }
        if (!orig.copy(destFile.absoluteFilePath())) {
            qWarning() << "copy database:" << orig.errorString();
            return QString();
        }
        if (!QFile::setPermissions(destFile.absoluteFilePath(), QFile::WriteOwner | QFile::ReadOwner)) {
            qWarning() << "database file setPermissions() failed!";
            return QString();
        }
        QFile c2file(c2.absoluteFilePath());
        c2file.open(QIODevice::WriteOnly | QIODevice::Text);
        c2file.write(c1txt.toUtf8());
        c2file.close();
    }
    qDebug() << "Database file:" << destFile.absoluteFilePath();
    return destFile.absoluteFilePath();
}

int main(int argc, char **argv)
{
    QGuiApplication::setApplicationName("barnatrees");
    QGuiApplication::setOrganizationName("BarcelonaTrees");
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setApplicationVersion(STRINGIFY(APPVER));
    QGuiApplication app(argc,argv);
    app.setWindowIcon(QIcon("qrc:/barnatrees_icon64.png"));
    QIcon::setThemeName("example");

    qDebug()<<"version:" << STRINGIFY(APPVER) << STRINGIFY(GITVER);
    //qDebug()<<"SSL version use for build: "<<QSslSocket::sslLibraryBuildVersionString();
    //qDebug()<<"SSL version use for run-time: "<<QSslSocket::sslLibraryVersionString();
    //qDebug()<<QCoreApplication::libraryPaths();

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

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));
    QString dbFile = localDatabaseFile();
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
    SummaryModel summaryModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gitversion", STRINGIFY(GITVER));
    engine.rootContext()->setContextProperty("availableStyles", QQuickStyle::availableStyles());
    engine.rootContext()->setContextProperty("speciesModel", &speciesModel);
    engine.rootContext()->setContextProperty("plantModel", &plantModel);
    engine.rootContext()->setContextProperty("summaryModel", &summaryModel);
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    QObject::connect(&engine, &QQmlApplicationEngine::exit, &app, &QCoreApplication::quit);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QCoreApplication::quit);
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
