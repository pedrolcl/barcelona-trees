#ifndef PLANTMODEL_H
#define PLANTMODEL_H
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
#include <QSqlRelationalTableModel>
#include <QGeoCoordinate>
#include "basemodel.h"

class PlantModel : public QSqlRelationalTableModel, public BaseModel
{
	Q_OBJECT
public:
	PlantModel(QObject *parent = nullptr, QSqlDatabase db = QSqlDatabase());
    ~PlantModel() override;

	Q_INVOKABLE QVariant data( const QModelIndex & index, int role = Qt::DisplayRole ) const override;
    QHash<int, QByteArray> roleNames() const override;
	QString selectStatement() const override;
	bool select() override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Q_INVOKABLE void setCenter(QGeoCoordinate center);
    Q_INVOKABLE void setSpecies(int idSpecies);
    Q_INVOKABLE void setGender(QString gen);
    Q_INVOKABLE void setStreet(QString street);
    Q_INVOKABLE QGeoCoordinate plantCoordinate(int row) const;
    Q_INVOKABLE qreal plantDistance(int row) const;
    Q_INVOKABLE QString formattedDistance(int row) const;
    Q_INVOKABLE QString formattedScientificName(int row) const;
    Q_INVOKABLE QString wikiLink(int row) const;
    Q_INVOKABLE QString streetLink(int row) const;

	int limit() const;
	void setLimit(int limit);

private:
	QGeoCoordinate CoordinateToCoordinate(QGeoCoordinate point, double range, double bearing) const;
    QGeoCoordinate m_center, m_p1, m_p2, m_p3, m_p4; //, m_nearestCoordinate;
	int m_limit;
};

#endif // PLANTMODEL_H
