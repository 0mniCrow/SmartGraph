#ifndef GVIEW_TABLEPHANTOMROWPROXYMODEL_H
#define GVIEW_TABLEPHANTOMROWPROXYMODEL_H
#include "gview_tableVertexModel.h"
#include <QSortFilterProxyModel>

class PhantomRowProxyModel: public QSortFilterProxyModel
{
    Q_OBJECT
private:
    int _phantom_row_;
public:
    PhantomRowProxyModel(QObject* tata = nullptr);
    void setPhantomRow(int row);
    void clearPhantomRow();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif // GVIEW_TABLEPHANTOMROWPROXYMODEL_H
