#ifndef PLANTPROXYMODEL_H
#define PLANTPROXYMODEL_H
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
#include <QSortFilterProxyModel>
#include "plantmodel.h"

class PlantProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    PlantProxyModel(PlantModel* source, QObject* parent = 0);
    Q_INVOKABLE QGeoCoordinate nearestPlantCoordinate() const;
    Q_INVOKABLE int nearestRow() const;
    Q_INVOKABLE int sourceRow(int row) const;
};

#endif // PLANTPROXYMODEL_H
