#include "basemodel.h"

BaseModel::BaseModel()
{ }

void BaseModel::generateRoleNames(const QSqlRecord& rec)
{
	m_roleNames.clear();
	for (int i = 0; i < rec.count(); i ++) {
		QByteArray roleName = rec.fieldName(i).toUtf8();
		int r = Qt::UserRole + i + 1;
		m_roleNames.insert(r, roleName);
		//qDebug() << "role" << r << roleName;
	}
}
