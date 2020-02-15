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
