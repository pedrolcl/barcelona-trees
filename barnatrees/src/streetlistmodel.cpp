#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "streetlistmodel.h"

StreetListModel::StreetListModel(): QStringListModel() {
    addQueryData("SELECT DISTINCT lower(substr(address, POS1+1, POS2-POS1-1)) as street "
                 "FROM (SELECT trim(plantAddress) as address, instr(trim(plantAddress)||' ',' ') as POS1, instr(trim(plantAddress)||',',',') as POS2 FROM plants) "
                 "ORDER BY 1;");
}

void StreetListModel::addQueryData(const QString sqlqry)
{
    QStringList names;
    QSqlQuery qry(sqlqry);
    if (qry.exec()) {
        while(qry.next()) {
            QString val = qry.value(0).toString();
            if (val.length() > 2) {
                names << val.remove(QRegExp("^(d'|de|del|dels|de\\sla)")).trimmed();
            }
        }
        names.removeDuplicates();
        names.sort();
        setStringList(names);
        //qDebug() << Q_FUNC_INFO << "rows: " << names.length();
    } else {
        qWarning() << Q_FUNC_INFO << qry.lastError();
    }
}

FilterProxyModel::FilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setFilterRole(Qt::DisplayRole);
}

void FilterProxyModel::setFilterString(QString string)
{
    QString filter = QString("^%1.*$").arg(string);
    setFilterRegExp(filter);
    //qDebug() << Q_FUNC_INFO << "row count:" << rowCount();
}
