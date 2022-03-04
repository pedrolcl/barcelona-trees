/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include <QDebug>
#include "plantproxymodel.h"

PlantProxyModel::PlantProxyModel(PlantModel* source, QObject* parent):
    QSortFilterProxyModel(parent)
{
    setSourceModel(source);
    setSortRole(source->lastRole() + 1);
    sort(source->lastRole() - Qt::UserRole);
}

int PlantProxyModel::sourceRow(int row) const
{
    return mapToSource(index(row, 0)).row();
}

int PlantProxyModel::nearestRow() const
{
    return sourceRow(0);
}

QGeoCoordinate PlantProxyModel::nearestPlantCoordinate() const
{
    QModelIndex first = index(0, 0);
    int rlon = roleNames().key("longitude");
    int rlat = roleNames().key("latitude");
    return QGeoCoordinate(first.data(rlat).toDouble(), first.data(rlon).toDouble());
}
