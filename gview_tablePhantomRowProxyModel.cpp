#include "gview_tablePhantomRowProxyModel.h"
PhantomRowProxyModel::PhantomRowProxyModel(QObject* tata):
    QSortFilterProxyModel(tata),_phantom_row_(-1)
{
    return;
}
void PhantomRowProxyModel::setPhantomRow(int row)
{
    _phantom_row_ = row;
    invalidate();
    return;
}
void PhantomRowProxyModel::clearPhantomRow()
{
    _phantom_row_ = -1;
    invalidate();
    return;
}
int PhantomRowProxyModel::rowCount(const QModelIndex& parent) const
{
    int base_count = QSortFilterProxyModel::rowCount(parent);
    return base_count + (_phantom_row_ >= 0 ? 1 : 0);
}
QModelIndex PhantomRowProxyModel::mapToSource(const QModelIndex& proxyIndex) const
{
    int row = proxyIndex.row();
    if(_phantom_row_ >= 0 && row>_phantom_row_)
    {
        --row;
    }
    return QSortFilterProxyModel::index(row,proxyIndex.column());
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
    return QSortFilterProxyModel::data(index,role);
}
Qt::ItemFlags PhantomRowProxyModel::flags(const QModelIndex& index) const
{
    if(_phantom_row_>=0 && index.row()==_phantom_row_)
    {
        return Qt::ItemIsEnabled;
    }
    return QSortFilterProxyModel::flags(index);
}
