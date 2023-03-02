/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include <QDebug>
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "jsonparser.h"
#include "treedata.h"

JsonParser::JsonParser(QObject *parent) : QObject(parent)
{
    m_pendingDatasets.clear();
    m_plantsDB.clear();
    m_plantsDataset.clear();
    m_speciesDB.clear();
    m_speciesDataset.clear();
}

void JsonParser::addDataset(OpenDataset ds)
{
    m_pendingDatasets.append(ds);
}

void JsonParser::execute()
{
    for(OpenDataset& ds : m_pendingDatasets) {
        QString f = ds.fileName;
        qDebug() << "processing:" << f;
        parse(ds);
    }
    updateDB();
    emit done();
}

void JsonParser::parse(OpenDataset& ds)
{
    m_plantsDataset.clear();
    m_speciesDataset.clear();
    QFile file(ds.fileName);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        file.close();
        m_db.transaction();
        parseJson(data);
        ds.species = m_speciesDataset.count();
        ds.specimens = m_plantsDataset.count();
        update(ds);
        m_db.commit();
        qDebug() << "number of trees:" << ds.specimens << "number of species:" << ds.species;
    } else {
        qWarning() << "file missing:" << ds.fileName;
    }
}

void JsonParser::parseJson(const QByteArray& text)
{
    QJsonDocument doc = QJsonDocument::fromJson(text);
    //qDebug() << Q_FUNC_INFO << doc.isEmpty() << doc.isNull() << doc.isObject() << doc.isArray();
    if (doc.isArray()) {
        QJsonArray results = doc.array();
        int resCount = results.count();
        if (!results.isEmpty()) {
            //qDebug() << "results:" << resCount;
            for(int i=0; i<resCount; ++i) {
                QJsonObject tree = results[i].toObject();
                TreeData a;
                a.setCodi( tree["codi"].toString() );
                a.setAdreca( tree["adreca"].toString() );
                a.setCatEspecieId( tree["cat_especie_id"].toInt() );
                QVariant lat = tree["latitud"].toVariant();
                QVariant lon = tree["longitud"].toVariant();
                a.setLatitud( lat.toReal() );
                a.setLongitud( lon.toReal() );
                a.setNomCientific( tree["nom_cientific"].toString() );
                a.setNomEsp( tree["nom_castella"].toString() );
                a.setNomCat( tree["nom_catala"].toString() );
                //qDebug() << a.codi() << a.adreca() << a.latitud() << a.longitud();
                process(a);
            }
        }
    }
}

void JsonParser::process(const TreeData& data)
{
    QSqlQuery q;

    int countBefore = m_speciesDB.count();
    m_speciesDB.insert(data.catEspecieId());
    m_speciesDataset.insert(data.catEspecieId());
    if (countBefore < m_speciesDB.count()) {
        if (!q.prepare("insert or ignore into species(idSpecies,scientificName,commonName1,commonName2) values(:id,:sn,:cn1,:cn2)")) {
            qWarning() << q.lastError();
        } else {
            q.bindValue(":id", data.catEspecieId());
            q.bindValue(":sn", data.nomCientific());
            q.bindValue(":cn1", data.nomEsp());
            q.bindValue(":cn2", data.nomCat());
            if (!q.exec()) {
                qWarning() << q.lastError();
            }
        }
    }

    countBefore = m_plantsDB.count();
    m_plantsDB.insert(data.codi());
    m_plantsDataset.insert(data.codi());
    if (countBefore < m_plantsDB.count()) {
        if (!q.prepare("insert or ignore into plants(idPlant,idSpecies,plantAddress,longitude,latitude) values(:idp,:ids,:pa,:lon,:lat)")) {
            qWarning() << q.lastError();
        } else {
            q.bindValue(":idp", data.codi());
            q.bindValue(":ids", data.catEspecieId());
            q.bindValue(":pa", data.adreca());
            q.bindValue(":lon", data.longitud());
            q.bindValue(":lat", data.latitud());
            if (!q.exec()) {
                qWarning() << q.lastError();
            }
        }
    }
}

void JsonParser::update(OpenDataset &ds)
{
    QSqlQuery q;
    if (!q.prepare("insert or ignore into summary(name,description,specimens,species) values(:name,:desc,:plants,:species)")) {
        qWarning() << q.lastError();
    } else {
        q.bindValue(":name", ds.name);
        q.bindValue(":desc", ds.time.toString(Qt::ISODate));
        q.bindValue(":plants", ds.specimens);
        q.bindValue(":species", ds.species);
        if (!q.exec()) {
            qWarning() << q.lastError();
        }
    }
}

void JsonParser::initDB()
{
    QFile file("barnatrees.db");
    if (file.exists()) {
        QString now = QDateTime::currentDateTimeUtc().toString("yyyyMMdd_hhmmss");
        file.rename(QString("barnatrees_%1.db").arg(now));
    }

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("barnatrees.db");
    if (!m_db.open()) {
        qWarning() << m_db.lastError();
        return;
    }

    QStringList tables = m_db.tables();
    if (tables.contains("species", Qt::CaseInsensitive) &&
            tables.contains("plants", Qt::CaseInsensitive) &&
            tables.contains("summary", Qt::CaseInsensitive))
    {
        qWarning() << "Database is already initialized";
        return;
    }

    QSqlQuery q;

    if (!q.exec(QLatin1String("PRAGMA foreign_keys=OFF")))
    {
        qWarning() << m_db.lastError();
        return;
    }

    if (!q.exec(QLatin1String("CREATE TABLE species (idSpecies INTEGER NOT NULL, scientificName TEXT NOT NULL, commonName1 TEXT, commonName2 TEXT, numSpecimens INTEGER, PRIMARY KEY(idSpecies))")))
    {
        qWarning() << q.lastError();
        return;
    }

    if (!q.exec(QLatin1String("CREATE TABLE plants (idPlant TEXT NOT NULL, idSpecies INTEGER NOT NULL, plantAddress TEXT, longitude REAL NOT NULL, latitude REAL NOT NULL, PRIMARY KEY(idPlant), FOREIGN KEY(idSpecies) REFERENCES species(idSpecies))")))
    {
        qWarning() << q.lastError();
        return;
    }

    if (!q.exec(QLatin1String("CREATE TABLE summary (name TEXT, description TEXT, specimens	INTEGER, species INTEGER, PRIMARY KEY(name))")))
    {
        qWarning() << q.lastError();
        return;
    }
}

void JsonParser::updateDB()
{
    QSqlQuery q;
    if (!q.exec(QLatin1String("CREATE INDEX coord1 ON plants ( longitude, latitude )")))
    {
        qWarning() << q.lastError();
    }

    if (!q.exec(QLatin1String("CREATE INDEX coord2 ON plants ( latitude, longitude )")))
    {
        qWarning() << q.lastError();
    }

    if (!q.exec("update species set numSpecimens = (select count(plants.idPlant) from plants where plants.idSpecies = species.idSpecies)")) {
        qWarning() << q.lastError();
    }

    if (!q.prepare("insert or ignore into summary(name,description,specimens,species) values(:name,:desc,:plants,:species)")) {
        qWarning() << q.lastError();
    } else {
        q.bindValue(":name", qApp->applicationName() + " v" + qApp->applicationVersion());
        q.bindValue(":desc", QDateTime::currentDateTimeUtc().toString(Qt::ISODate));
        q.bindValue(":plants", m_plantsDB.count());
        q.bindValue(":species", m_speciesDB.count());
        if (!q.exec()) {
            qWarning() << q.lastError();
        }
    }

    if (!q.exec(QLatin1String("PRAGMA foreign_keys=ON")))
    {
        qWarning() << q.lastError();
    }
}
