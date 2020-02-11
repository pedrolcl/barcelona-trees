#ifndef STREETLISTMODEL_H
#define STREETLISTMODEL_H

#include <QStringListModel>
#include <QSortFilterProxyModel>

class StreetListModel : public QStringListModel
{
public:
    StreetListModel();
    void addQueryData(const QString sqlqry);
};

class FilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    FilterProxyModel(QObject* parent = 0);
    Q_INVOKABLE void setFilterString(QString string);
};

#endif // STREETLISTMODEL_H
