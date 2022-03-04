#ifndef BASEMODEL_H
#define BASEMODEL_H

/*
Barcelona Trees; a guide of the trees of Barcelona
Copyright (C) 2019-2022 Pedro Lopez-Cabanillas <plcl@users.sf.net>

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

#include <QHash>
#include <QByteArray>
#include <QSqlRecord>

class BaseModel
{
public:
	BaseModel();
    int lastRole() const { return m_lastRole; }

protected:
	void generateRoleNames(const QSqlRecord& rec);

	QHash<int, QByteArray> m_roleNames;
    int m_lastRole;
};

#endif // BASEMODEL_H
