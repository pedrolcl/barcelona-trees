#include <QDebug>
#include <QSqlError>
#include "speciesmodel.h"

SpeciesModel::SpeciesModel(QObject *parent)
	: QSqlTableModel(parent)
{
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setTable("species");
	setSort(1, Qt::AscendingOrder);
	//qDebug() << selectStatement();
	if (select()) {
		generateRoleNames(record());
	} else {
		qWarning() << lastError();
	}
}

QHash<int, QByteArray> SpeciesModel::roleNames() const
{
	return m_roleNames;
}

QVariant SpeciesModel::data(const QModelIndex &index, int role) const
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

QVariant SpeciesModel::data(int row, const QString & fieldName) const
{
	QVariant value;
	int col = record().indexOf(fieldName);
	if(col != -1 && 0 <= row && row < rowCount()) {
		QModelIndex ix = this->index(row, col);
		value = ix.data();
	}
	return value;
}

bool SpeciesModel::select()
{
	bool res = QSqlTableModel::select();
	if (res) {
		while(canFetchMore()) {
			fetchMore();
		}
        //qDebug() << "species.rows:" << rowCount();
	}
	return res;
}
