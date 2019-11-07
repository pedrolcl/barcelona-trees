#include <QDebug>
#include <QSqlError>
#include "summarymodel.h"

SummaryModel::SummaryModel(QObject *parent)
    : QSqlTableModel(parent)
{
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    setTable("summary");
    setSort(1, Qt::AscendingOrder);
    //qDebug() << selectStatement();
    if (select()) {
        generateRoleNames(record());
    } else {
        qWarning() << lastError();
    }
}

QHash<int, QByteArray> SummaryModel::roleNames() const
{
    return m_roleNames;
}

QVariant SummaryModel::data(const QModelIndex &index, int role) const
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

QVariant SummaryModel::data(int row, const QString & fieldName) const
{
    QVariant value;
    int col = record().indexOf(fieldName);
    if(col != -1 && 0 <= row && row < rowCount()) {
        QModelIndex ix = this->index(row, col);
        value = ix.data();
    }
    return value;
}

bool SummaryModel::select()
{
    bool res = QSqlTableModel::select();
    if (res) {
        while(canFetchMore()) {
            fetchMore();
        }
        //qDebug() << "summary.rows:" << rowCount();
    }
    return res;
}
