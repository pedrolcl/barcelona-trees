#ifndef GENDERMODEL_H
#define GENDERMODEL_H

#include <QSqlTableModel>
#include "basemodel.h"

class GenderModel : public QSqlTableModel, public BaseModel
{
    Q_OBJECT

public:
    explicit GenderModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;
    QString selectStatement() const override;

    Q_INVOKABLE QVariant data(int row, const QString & fieldName) const;
};

#endif // GENDERMODEL_H
