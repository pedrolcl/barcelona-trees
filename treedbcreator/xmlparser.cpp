#include <QObject>
#include <QDebug>
#include <QCoreApplication>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "xmlparser.h"
#include "treedata.h"

XmlParser::XmlParser(QObject *parent) : QObject(parent)
{
    m_pendingDatasets.clear();
    m_plantsDB.clear();
    m_plantsDataset.clear();
    m_speciesDB.clear();
    m_speciesDataset.clear();
}

void XmlParser::addDataset(OpenDataset ds)
{
    m_pendingDatasets.append(ds);
}

void XmlParser::execute()
{
    for(OpenDataset& ds : m_pendingDatasets) {
        QString f = ds.fileName;
        qDebug() << "processing:" << f;
        parse(ds);
    }
    updateDB();
    emit done();
}

void XmlParser::parse(OpenDataset& ds)
{
    m_plantsDataset.clear();
    m_speciesDataset.clear();
    m_xmlData.clear();
    QFile file(ds.fileName);
    if (file.exists()) {
        file.open(QIODevice::ReadOnly);
        m_xmlData = file.readAll();
        file.close();
        m_db.transaction();
        parseXml();
        ds.species = m_speciesDataset.count();
        ds.specimens = m_plantsDataset.count();
        update(ds);
        m_db.commit();
        qDebug() << "number of trees:" << ds.specimens << "number of species:" << ds.species;
    } else {
        qWarning() << "file missing:" << ds.fileName;
    }
}

void XmlParser::parseXml()
{
    QXmlStreamReader reader(m_xmlData);
    while(!reader.atEnd() && !reader.hasError()) {
        TreeData a;
        reader.readNext();
        if (reader.isStartElement()) {
            if (reader.name() == "llistatArbrat") {
                reader.readNext();
            }
            while (reader.isWhitespace() || reader.isComment()) {
                //qDebug() << "001 junk place" << reader.text();
                reader.readNext();
            }
            while (reader.isStartElement()) {
                if (reader.name() == "arbre") {
                    a.clear();
                    QVariant tmp;
                    reader.readNext();
                    while (reader.isWhitespace() || reader.isComment()) {
                        //qDebug() << "002 junk place" << reader.text();
                        reader.readNext();
                    }
                    while (reader.isStartElement()) {
                        if (reader.name() == "codi") {
                            a.setCodi( reader.readElementText() );
                            //qDebug() << "codi:" << a->codi();
                            reader.readNext();
                        } else if (reader.name() == "posicioX_ETRS89") {
                            tmp = reader.readElementText();
                            a.setPosicioX_ETRS89(tmp.toReal());
                            reader.readNext();
                        } else if (reader.name() == "posicioY_ETRS89") {
                            tmp = reader.readElementText();
                            a.setPosicioY_ETRS89(tmp.toReal());
                            reader.readNext();
                        } else if (reader.name() == "latitud_WGS84") {
                            tmp = reader.readElementText();
                            a.setLatitud_WGS84(tmp.toReal());
                            reader.readNext();
                        } else if (reader.name() == "longitud_WGS84") {
                            tmp = reader.readElementText();
                            a.setLongitud_WGS84(tmp.toReal());
                            reader.readNext();
                        } else if (reader.name() == "tipusElement") {
                            a.setTipusElement(reader.readElementText());
                            reader.readNext();
                        } else if (reader.name() == "espaiVerd") {
                            a.setEspaiVerd( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "adreca") {
                            a.setAdreca( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "alcada") {
                            a.setAlcada( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "catEspecieId") {
                            tmp = reader.readElementText();
                            a.setCatEspecieId(tmp.toInt());
                            reader.readNext();
                        } else if (reader.name() == "nomCientific") {
                            a.setNomCientific( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "nomEsp") {
                            a.setNomEsp( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "nomCat") {
                            a.setNomCat( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "categoriaArbrat") {
                            a.setCategoriaArbrat( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "ampladaVorera") {
                            a.setAmpladaVorera( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "plantacioDT") {
                            a.setPlantacioDT( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "tipAigua") {
                            a.setTipAigua( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "tipReg") {
                            a.setTipReg( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "tipSuperf") {
                            a.setTipSuperf( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "tipSuport") {
                            a.setTipSuport( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "cobertaEscocell") {
                            a.setCobertaEscocell( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "midaEscocell") {
                            a.setMidaEscocell( reader.readElementText() );
                            reader.readNext();
                        } else if (reader.name() == "voraEscocell") {
                            a.setVoraEscocell( reader.readElementText() );
                            reader.readNext();
                        }
                        reader.readNext();
                        while (reader.isWhitespace() || reader.isComment()) {
                            //qDebug() << "003 junk place" << reader.text();
                            reader.readNext();
                        }
                    }
                    process(a);
                    reader.readNext();
                    while (reader.isWhitespace() || reader.isEndElement() || reader.isComment()) {
                        //qDebug() << "004 junk place" << reader.text();
                        reader.readNext();
                    }
                }
            }
        }
    }
    if(reader.hasError()) {
        qDebug() << "xmlFile.xml Parse Error" << reader.errorString();
    }
    //close reader and flush file
    reader.clear();
}

void XmlParser::process(const TreeData& data)
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
            q.bindValue(":lon", data.longitud_WGS84());
            q.bindValue(":lat", data.latitud_WGS84());
            if (!q.exec()) {
                qWarning() << q.lastError();
            }
        }
    }
}

void XmlParser::update(OpenDataset &ds)
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

void XmlParser::initDB()
{
    QFile file("barnatrees.db");
    if (file.exists()) {
        QString now = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
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

void XmlParser::updateDB()
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
        q.bindValue(":desc", QDateTime::currentDateTime().toString(Qt::ISODate));
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
