#ifndef BASEMODEL_H
#define BASEMODEL_H

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
