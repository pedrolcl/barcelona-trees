#ifndef SPECIESMODEL_H
#define SPECIESMODEL_H
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
#include <QSqlTableModel>
#include "basemodel.h"

class SpeciesModel : public QSqlTableModel, public BaseModel
{
	Q_OBJECT

public:
	explicit SpeciesModel(QObject *parent = nullptr);

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;
	bool select() override;

	Q_INVOKABLE QVariant data(int row, const QString & fieldName) const;
};

#endif // SPECIESMODEL_H
