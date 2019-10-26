#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include "xmlparser.h"
#include "downloadmanager.h"

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	XmlParser parser;
	DownloadManager manager;

    //manager.addDownload("https://opendata-ajuntament.barcelona.cat/resources/bcn/arbrat_zona.xml");
    //manager.addDownload("https://opendata-ajuntament.barcelona.cat/resources/bcn/arbrat_viari.xml");
    manager.addDownload("https://opendata-ajuntament.barcelona.cat/data/dataset/9b525e1d-13b8-48f1-abf6-f5cd03baa1dd/resource/4c49c634-8fbf-4ecc-b6a0-c448268271ba/download");
    manager.addDownload("https://opendata-ajuntament.barcelona.cat/data/dataset/27b3f8a7-e536-4eea-b025-ce094817b2bd/resource/32f4edc5-41cb-40b1-89f7-465435694df4/download");

	QObject::connect(&manager, &DownloadManager::fileReady, &parser, &XmlParser::addFilename);
	QObject::connect(&manager, &DownloadManager::done, &parser, &XmlParser::execute);
	QObject::connect(&parser, &XmlParser::done, &app, &QCoreApplication::quit);

	QTimer::singleShot(0, &manager, SLOT(execute()));
	parser.initDB();

	app.exec();
}
