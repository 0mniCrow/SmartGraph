#ifndef GVIEW_TABLEPHANTOMROWPROXYMODEL_H
#define GVIEW_TABLEPHANTOMROWPROXYMODEL_H
#include "gview_tableVertexModel.h"
#include <QSortFilterProxyModel>

class PhantomRowProxyModel: public QAbstractProxyModel
{
    Q_OBJECT
private:
    int _phantom_row_;
    int _dragged_row_;
    QObject * _parent_;
public:
    PhantomRowProxyModel(QObject* tata = nullptr);
    void setPhantomRow(int row);
    void clearPhantomRow();
    bool isPhantomRowSet() const;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    QModelIndex mapToSource(const QModelIndex& proxyIndex) const override;
    QModelIndex mapFromSource(const QModelIndex& sourceIndex) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex())const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action,
                      int row, int column, const QModelIndex& parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;
    QStringList mimeTypes() const override;
    QMimeData* mimeData(const QModelIndexList& indexes) const override;

    VertexModel* getSourceModel();
};

#endif // GVIEW_TABLEPHANTOMROWPROXYMODEL_H
