#ifndef SUMMARYMODEL_H
#define SUMMARYMODEL_H

#include <QObject>
#include <QSqlTableModel>
#include "basemodel.h"

class SummaryModel : public QSqlTableModel, public BaseModel
{
    Q_OBJECT
public:
    explicit SummaryModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool select() override;

    Q_INVOKABLE QVariant data(int row, const QString & fieldName) const;
};

#endif // SUMMARYMODEL_H
