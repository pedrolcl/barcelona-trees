/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2023 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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
#include "basemodel.h"

BaseModel::BaseModel()
{ }

void BaseModel::generateRoleNames(const QSqlRecord& rec)
{
	m_roleNames.clear();
    for (int i = 0; i < rec.count(); i++) {
		QByteArray roleName = rec.fieldName(i).toUtf8();
        m_lastRole = Qt::UserRole + i + 1;
        m_roleNames.insert(m_lastRole, roleName);
        //qDebug() << "role" << r << roleName;
	}
}
