#include <QDebug>
#include <QSqlError>
#include "gendermodel.h"

GenderModel::GenderModel(QObject *parent): QSqlTableModel(parent)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    //qDebug() << selectStatement();
    if (select()) {
        generateRoleNames(record());
    } else {
        qWarning() << lastError();
    }
}

QHash<int, QByteArray> GenderModel::roleNames() const
{
    return m_roleNames;
}

QVariant GenderModel::data(const QModelIndex &index, int role) const
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

QVariant GenderModel::data(int row, const QString & fieldName) const
{
    QVariant value;
    int col = record().indexOf(fieldName);
    if(col != -1 && 0 <= row && row < rowCount()) {
        QModelIndex ix = this->index(row, col);
        value = ix.data();
    }
    return value;
}

bool GenderModel::select()
{
    bool res = QSqlTableModel::select();
    if (res) {
        while(canFetchMore()) {
            fetchMore();
        }
        //qDebug() << "gender.rows:" << rowCount();
    }
    return res;
}

QString GenderModel::selectStatement() const
{
    return QString("SELECT DISTINCT substr(trim(scientificName),1,instr(trim(scientificName)||' ',' ')-1) AS gender FROM species ORDER BY gender");
}
