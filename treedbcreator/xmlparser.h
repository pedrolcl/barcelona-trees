#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QByteArray>
#include <QSqlDatabase>
#include "treedata.h"

class XmlParser: public QObject
{
	Q_OBJECT

public:
	explicit XmlParser(QObject *parent = nullptr);
	void initDB();

public slots:
	void execute();
	void addFilename(QString name);

signals:
	void done();

private:
	void parseXml();
	void readFile(const QString& name);
	void parse(const QString& name);
	void process(const TreeData& data);
	void updateDB();

	QStringList m_pendingFiles;
	QByteArray m_xmlData;
	QSqlDatabase m_db;
	int m_plants;
};

#endif // XMLPARSER_H
