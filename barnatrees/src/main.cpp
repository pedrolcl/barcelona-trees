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
    qDebug() << "locale:" << locale;
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

    QGeoCoordinate locationBarna = QGeoCoordinate( 41.403216, 2.186674 );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setConnectOptions(QLatin1String("QSQLITE_OPEN_READONLY"));
#if defined(Q_OS_ANDROID)
    QFileInfo orig("assets:/barnatrees.db");
    qDebug() << orig.path() << "exists:" << orig.exists();
    if (!orig.exists())
        return -1;
    QDir destDir = QDir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QFileInfo destFile(destDir, "barnatrees.db");
    if (!destFile.exists()) {
        qDebug() << "copy database:" << QFile::copy(orig.filePath(), destFile.absoluteFilePath());
        qDebug() << "database permissions:" << QFile::setPermissions(destFile.absoluteFilePath(),QFile::WriteOwner | QFile::ReadOwner) ;
    }
    db.setDatabaseName(destFile.absoluteFilePath());
#elif defined(Q_OS_MAC)
    db.setDatabaseName("../Resources/barnatrees.db");
#else
    db.setDatabaseName("barnatrees.db");
#endif
    if (!db.open()) {
        qCritical() << db.lastError();
        return -1;
    }

    SpeciesModel speciesModel;
    qDebug() << "species.columns:" << speciesModel.columnCount();
    PlantModel plantModel;
    plantModel.setCenter(locationBarna);
    qDebug() << "plants.columns:" << plantModel.columnCount();
    SummaryModel summaryModel;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("gitversion", STRINGIFY(GITVER));
    engine.rootContext()->setContextProperty("availableStyles", QQuickStyle::availableStyles());
    engine.rootContext()->setContextProperty("speciesModel", &speciesModel);
    engine.rootContext()->setContextProperty("plantModel", &plantModel);
    engine.rootContext()->setContextProperty("summaryModel", &summaryModel);
    engine.load(QUrl(QStringLiteral("qrc:/MainWindow.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }
    return app.exec();
}
