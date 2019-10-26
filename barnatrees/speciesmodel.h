#ifndef SPECIESMODEL_H
#define SPECIESMODEL_H

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
