#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <QObject>
#include <QByteArray>
#include <QSqlDatabase>
#include "treedata.h"
#include "downloadmanager.h"

class XmlParser: public QObject
{
    Q_OBJECT

public:
    explicit XmlParser(QObject *parent = nullptr);
    void initDB();

public slots:
    void execute();
    void addDataset(OpenDataset ds);

signals:
    void done();

private:
    void parseXml();
    void parse(OpenDataset& ds);
    void process(const TreeData& data);
    void update(OpenDataset& ds);
    void updateDB();

    QList<OpenDataset> m_pendingDatasets;
    QByteArray m_xmlData;
    QSqlDatabase m_db;

    QSet<QString> m_plantsDB;
    QSet<int> m_speciesDB;

    QSet<QString> m_plantsDataset;
    QSet<int> m_speciesDataset;
};

#endif // XMLPARSER_H
