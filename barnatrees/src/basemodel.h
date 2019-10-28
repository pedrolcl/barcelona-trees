#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QHash>
#include <QByteArray>
#include <QSqlRecord>

class BaseModel
{
public:
	BaseModel();

protected:
	void generateRoleNames(const QSqlRecord& rec);

	QHash<int, QByteArray> m_roleNames;
};

#endif // BASEMODEL_H
