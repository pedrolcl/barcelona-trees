#ifndef XMLPARSER_H
#define XMLPARSER_H
/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2020 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
