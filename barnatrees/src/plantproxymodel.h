#ifndef PLANTPROXYMODEL_H
#define PLANTPROXYMODEL_H

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
