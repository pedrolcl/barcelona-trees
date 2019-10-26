#include <QDebug>
#include <QGeoCoordinate>
#include <QSqlError>
#include <QSqlRecord>
#include <cmath>
#include "plantmodel.h"

PlantModel::PlantModel(QObject *parent, QSqlDatabase db): QSqlRelationalTableModel(parent, db)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("plants");
	setRelation(fieldIndex("idSpecies"), QSqlRelation("species", "idSpecies", "scientificName, commonName1, commonName2"));
	m_limit = 10000;
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
    QString filter = "scientificName like '" + gen +  "%'";
	//qDebug() << "filter:" << filter;
    setFilter(filter);
}

void PlantModel::setStreet(QString street)
{
    QString filter = "plantAddress like '%" + street + "%'";
	//qDebug() << "filter:" << filter;
	setFilter(filter);
}

void PlantModel::setSpecies(int idSpecies)
{
	QString filter = QString("plants.idSpecies = %1").arg(idSpecies);
	//qDebug() << "filter:" << filter;
    setFilter(filter);
}

QGeoCoordinate PlantModel::plantCoordinate(int row)
{
	QModelIndex lat_ix = this->index(row, record().indexOf("latitude"));
	QModelIndex lon_ix = this->index(row, record().indexOf("longitude"));
	return QGeoCoordinate(lat_ix.data().toReal(), lon_ix.data().toReal());
}

QGeoCoordinate PlantModel::nearestPlant()
{
	QGeoCoordinate nearest;
	double nearest_distance = 9e7;
    if (rowCount() == 0) {
		return QGeoCoordinate();
    } else if (rowCount() == 1) {
		return plantCoordinate(0);
    } else if (rowCount() >= 2 &&  rowCount() <= 200) {
		for(int i=0; i<rowCount(); ++i) {
			double d;
			QGeoCoordinate x = plantCoordinate(i);
			if ((d = m_center.distanceTo(x)) < nearest_distance) {
				nearest_distance = d;
				nearest = x;
			}
		}
		return nearest;
    } else {
		return plantCoordinate(rowCount() / 2);
	}
}

void PlantModel::setCenter(QGeoCoordinate center)
{
    m_center = center;
	double radius = 353.55; // square of ½km side
    m_p1 = CoordinateToCoordinate(center, radius, 0);
    m_p2 = CoordinateToCoordinate(center, radius, 90);
    m_p3 = CoordinateToCoordinate(center, radius, 180);
    m_p4 = CoordinateToCoordinate(center, radius, 270);

	//QString filter = QString("(latitude between %1 and %2) and (longitude between %3 and %4)").arg(m_p3.latitude()).arg(m_p1.latitude()).arg(m_p4.longitude()).arg(m_p2.longitude());
	QString filter = QString("latitude > %1 and latitude < %2 and longitude > %3 and longitude < %4").arg(m_p3.latitude()).arg(m_p1.latitude()).arg(m_p4.longitude()).arg(m_p2.longitude());
	//qDebug() << "filter:" << filter;
    setFilter(filter);
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
