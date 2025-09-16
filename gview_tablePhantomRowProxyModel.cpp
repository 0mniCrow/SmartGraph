#include "gview_tablePhantomRowProxyModel.h"
PhantomRowProxyModel::PhantomRowProxyModel(QObject* tata):
    QAbstractProxyModel(tata),_phantom_row_(-1)
{
    _parent_ = tata;
    return;
}
void PhantomRowProxyModel::setPhantomRow(int row)
{
    beginResetModel();
    _phantom_row_ = row;
    endResetModel();
    //invalidate();
    return;
}
void PhantomRowProxyModel::clearPhantomRow()
{
    beginResetModel();
    _phantom_row_ = -1;
    endResetModel();
    //invalidate();
    return;
}

bool PhantomRowProxyModel::isPhantomRowSet() const
{
    return _phantom_row_>=0;
}

VertexModel* PhantomRowProxyModel::getSourceModel()
{
    return dynamic_cast<VertexModel*>(sourceModel());
}

int PhantomRowProxyModel::rowCount(const QModelIndex& parent) const
{
    int base_count = sourceModel()->rowCount(mapToSource(parent));
    return base_count + (_phantom_row_ >= 0 ? 1 : 0);
}

int PhantomRowProxyModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return sourceModel()->columnCount(QModelIndex());
}
QModelIndex PhantomRowProxyModel::parent(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QModelIndex();
}
QModelIndex PhantomRowProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
    int row = proxyIndex.row();
    if(_phantom_row_ >= 0 && row>_phantom_row_)
    {
        --row;
    }
    return sourceModel()->index(row,proxyIndex.column());
}

QModelIndex PhantomRowProxyModel::mapFromSource(const QModelIndex& sourceIndex) const
{
    int row = sourceIndex.row();
    if(_phantom_row_ >= 0 && row >=_phantom_row_)
    {
        ++row;
    }
    return index(row,sourceIndex.column());
}
QVariant PhantomRowProxyModel::data(const QModelIndex& index, int role) const
{
    if(_phantom_row_>=0 && index.row()==_phantom_row_)
    {
        if(role == Qt::DisplayRole)
        {
            return QVariant("Drop here...");
        }
        else if(role == Qt::BackgroundRole)
        {
            return QColor(179,237,235);
        }
        return QVariant();
    }
    return sourceModel()->data(mapToSource(index),role);
}

 QModelIndex PhantomRowProxyModel::index(int row, int column, const QModelIndex& parent) const
 {
        Q_UNUSED(parent);
     return sourceModel()->index(row,column,QModelIndex());
 }
Qt::ItemFlags PhantomRowProxyModel::flags(const QModelIndex& index) const
{

    if (!index.isValid())
    {
            return Qt::ItemIsDropEnabled;
    }
    if(_phantom_row_>=0 && index.row()==_phantom_row_)
    {
        return Qt::ItemIsEnabled|Qt::ItemIsDropEnabled;
    }
    QModelIndex sourceIndex = mapToSource(index);
    Qt::ItemFlags fl = sourceModel()->flags(sourceIndex);
    //qDebug()<<"Row["<<index.row()<<"] Flags: "<<fl;

    return fl;//sourceModel()->flags(sourceIndex);
}

bool PhantomRowProxyModel::dropMimeData(const QMimeData* data,
                                        Qt::DropAction action,
                  int row, int column, const QModelIndex& parent)
{
    int source_row = row;
    if(_phantom_row_ >= 0 && row > _phantom_row_ )
    {
        --source_row;
    }
    QModelIndex sourceParent(mapToSource(parent));
    return sourceModel()->dropMimeData(data,action,source_row,column,sourceParent);
}

Qt::DropActions PhantomRowProxyModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions PhantomRowProxyModel::supportedDragActions() const
{
    return Qt::MoveAction;
}
QStringList PhantomRowProxyModel::mimeTypes() const
{
    return sourceModel()->mimeTypes();
}
QMimeData* PhantomRowProxyModel::mimeData(const QModelIndexList& indexes) const
{
    QModelIndexList sourceIndexes;
    for(const QModelIndex& proxy_index:indexes)
    {
        if(proxy_index.isValid()&&proxy_index.row()!=_phantom_row_)
        {
            sourceIndexes.append(mapToSource(proxy_index));
        }
    }
    return sourceModel()->mimeData(sourceIndexes);
}
