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
