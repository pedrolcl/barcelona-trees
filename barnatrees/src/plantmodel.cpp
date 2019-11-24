#include <QDebug>
#include <QGeoCoordinate>
#include <QSqlError>
#include <QSqlRecord>
#include <QLocale>
#include <QSettings>
#include <cmath>
#include "plantmodel.h"

PlantModel::PlantModel(QObject *parent, QSqlDatabase db):
    QSqlRelationalTableModel(parent, db),
    m_nearestRow(-1),
    m_limit(5000)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("plants");
	setRelation(fieldIndex("idSpecies"), QSqlRelation("species", "idSpecies", "scientificName, commonName1, commonName2"));
	//qDebug() << selectStatement();
    if (select()) {
		generateRoleNames(record());
    } else {
        qWarning() << lastError();
    }
}

PlantModel::~PlantModel()
{ }

QVariant PlantModel::data( const QModelIndex & index, int role ) const
{
    QVariant value;
    if (index.isValid()) {
        if (role < Qt::UserRole) {
            value = QSqlQueryModel::data(index, role);
        } else {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        }
    }
    return value;
}

QHash<int, QByteArray> PlantModel::roleNames() const
{
	return m_roleNames;
}

int PlantModel::limit() const
{
	return m_limit;
}

void PlantModel::setLimit(int limit)
{
    m_limit = limit;
}


QString PlantModel::selectStatement() const
{
	QString query = QSqlRelationalTableModel::selectStatement();
	if(m_limit > 0)
		query += QString(" LIMIT %1").arg(m_limit);
	return query;
}

bool PlantModel::select()
{
	bool res = QSqlRelationalTableModel::select();
	if (res) {
		while(canFetchMore()) {
			fetchMore();
		}
        qDebug() << "plants.rows:" << rowCount();
	}
	return res;
}

void PlantModel::setGender(QString gen)
{
    QString filter = "scientificName like '%" + gen +  "%'";
	//qDebug() << "filter:" << filter;
    setFilter(filter);
    calcNearestPlant();
}

void PlantModel::setStreet(QString street)
{
    QString filter = "plantAddress like '%" + street + "%'";
	//qDebug() << "filter:" << filter;
	setFilter(filter);
    calcNearestPlant();
}

void PlantModel::setSpecies(int idSpecies)
{
	QString filter = QString("plants.idSpecies = %1").arg(idSpecies);
	//qDebug() << "filter:" << filter;
    setFilter(filter);
    calcNearestPlant();
}

QGeoCoordinate PlantModel::plantCoordinate(int row)
{
	QModelIndex lat_ix = this->index(row, record().indexOf("latitude"));
	QModelIndex lon_ix = this->index(row, record().indexOf("longitude"));
	return QGeoCoordinate(lat_ix.data().toReal(), lon_ix.data().toReal());
}

qreal PlantModel::plantDistance(int row)
{
	return plantCoordinate(row).distanceTo(m_center);
}

QString PlantModel::formattedDistance(int row)
{
	QLocale locale;
	auto distance = plantDistance(row);
	if (distance < 1000.0)
		return locale.toString(distance, 'f', 0) + " m";
	auto km = distance/1000.0;
	if (km < 10.0)
		return locale.toString(km, 'f', 1) + " km";
	return locale.toString(km, 'f', 0) + " km";
}

QString PlantModel::formattedScientificName(int row)
{
	QModelIndex sn_ix = this->index(row, record().indexOf("scientificName"));
	QString scientificName = sn_ix.data().toString();
	int p = -1;
	if ((p = scientificName.indexOf(" x ")) > -1) {
		p = scientificName.indexOf(" ", p+3);
	} else if ((p = scientificName.indexOf(" ")) > -1) {
		p = scientificName.indexOf(" ", p+1);
	}
	if (p > -1) {
		return QString("<i>%1</i> %2").arg(scientificName.left(p)).arg(scientificName.mid(p+1));
	} else {
		return QString("<i>%1</i>").arg(scientificName);
	}
}

QString PlantModel::wikiLink(int row)
{
    QSettings settings;
    QString txt = formattedScientificName(row);
	QModelIndex sn_ix = this->index(row, record().indexOf("scientificName"));
	QString s = sn_ix.data().toString();
	int p = -1;
	if ((p = s.indexOf(" x ")) > -1) {
		s = s.replace(p, 3, "_x_");
	} else if ((p = s.indexOf(" ")) > -1) {
		s = s.replace(p, 1, "_");
	}
	p = s.indexOf(" ");
	if ( p > -1) {
		s = s.left(p);
	}
    QString linktype = settings.value("links", "Wikipedia").toString();
    QString lang = settings.value("language", QLocale().name().left(2)).toString();
    if (linktype == "Wikipedia") {
        return QString("<a href='https://%1.wikipedia.org/wiki/%2'>%3</a>").arg(lang).arg(s).arg(txt);
    } else if (linktype == "Species") {
        return QString("<a href='https://species.wikimedia.org/wiki/%1'>%2</a>").arg(s).arg(txt);
    } else {
        return QString("<a href='https://commons.wikimedia.org/wiki/%1'>%2</a>").arg(s).arg(txt);
    }
}

QString PlantModel::streetLink(int row)
{
	QModelIndex lat_ix = this->index(row, record().indexOf("latitude"));
	QModelIndex lon_ix = this->index(row, record().indexOf("longitude"));
	QModelIndex addr_ix = this->index(row, record().indexOf("plantAddress"));
	auto pos = QString("viewpoint=%1,%2").arg(lat_ix.data().toReal()).arg(lon_ix.data().toReal());
	return QString("<a href='https://www.google.com/maps/@?api=1&map_action=pano&%1'>%2</a>").arg(pos).arg(addr_ix.data().toString());
}

void PlantModel::calcNearestPlant()
{
    QGeoCoordinate nearest;
    double nearest_distance = 9e7;
    if (rowCount() == 0) {
        m_nearestRow = -1;
        m_nearestCoordinate = QGeoCoordinate();
    } else if (rowCount() == 1) {
        m_nearestRow = 0;
        m_nearestCoordinate = plantCoordinate(0);
    } else if (rowCount() >= 2 &&  rowCount() <= 200) {
        for(int i=0; i<rowCount(); ++i) {
            double d;
            QGeoCoordinate x = plantCoordinate(i);
            if ((d = m_center.distanceTo(x)) < nearest_distance) {
                nearest_distance = d;
                nearest = x;
                m_nearestRow = i;
            }
        }
        m_nearestCoordinate = nearest;
    } else {
        m_nearestRow = rowCount() / 2;
        m_nearestCoordinate = plantCoordinate(m_nearestRow);
    }
    qDebug() << "nearest row:" << m_nearestRow;
}

QGeoCoordinate PlantModel::nearestPlantCoordinate()
{
    return m_nearestCoordinate;
}

int PlantModel::nearestRow()
{
    return m_nearestRow;
}

void PlantModel::setCenter(QGeoCoordinate center)
{
    m_center = center;
    double radius = 250.0; // square of 500 m side
	m_p1 = CoordinateToCoordinate(center, radius, 0);
    m_p2 = CoordinateToCoordinate(center, radius, 90);
    m_p3 = CoordinateToCoordinate(center, radius, 180);
    m_p4 = CoordinateToCoordinate(center, radius, 270);

    QString filter = QString("(latitude between %1 and %2) and (longitude between %3 and %4)").arg(m_p3.latitude()).arg(m_p1.latitude()).arg(m_p4.longitude()).arg(m_p2.longitude());
    //QString filter = QString("latitude > %1 and latitude < %2 and longitude > %3 and longitude < %4").arg(m_p3.latitude()).arg(m_p1.latitude()).arg(m_p4.longitude()).arg(m_p2.longitude());
    //qDebug() << "filter:" << filter;
    setFilter(filter);
    calcNearestPlant();
}

static const double PI = 3.14159265358979323846, earthDiameterMeters = 6371.0 * 2 * 1000;
static double degreeToRadian (const double degree) { return (degree * PI / 180); };
static double radianToDegree (const double radian) { return (radian * 180 / PI); };

QGeoCoordinate PlantModel::CoordinateToCoordinate (QGeoCoordinate center, double meters, double angle) const
{
  double latitude = degreeToRadian(center.latitude());
  double longitude = degreeToRadian(center.longitude());
  angle = degreeToRadian(angle);
  meters *= 2 / earthDiameterMeters;

  using namespace std;
  double lat = asin((sin(latitude) * cos(meters)) + (cos(latitude) * sin(meters) * cos(angle)));
  double lon = longitude + atan2( (sin(angle) * sin(meters) * cos(latitude)),
                                   cos(meters) - (sin(latitude) * sin(lat)) );
  lat = radianToDegree(lat);
  lon = radianToDegree(lon);
  return QGeoCoordinate(lat, lon);
}
