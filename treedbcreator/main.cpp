#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include "xmlparser.h"
#include "downloadmanager.h"

#define _STR(x) #x
#define STRINGIFY(x) _STR(x)

int main(int argc, char *argv[])
{
    QCoreApplication::setApplicationName("treedbcreator");
    QCoreApplication::setOrganizationName("BarcelonaTrees");
    QCoreApplication::setApplicationVersion(STRINGIFY(APPVER));
    QCoreApplication app(argc, argv);

    XmlParser parser;
    DownloadManager manager;
    QObject::connect(&manager, &DownloadManager::downloadReady, &parser, &XmlParser::addDataset);
    QObject::connect(&manager, &DownloadManager::done, &parser, &XmlParser::execute);
    QObject::connect(&parser, &XmlParser::done, &app, &QCoreApplication::quit);

    QTimer::singleShot(0, &manager, SLOT(execute()));
    parser.initDB();

    app.exec();
}
