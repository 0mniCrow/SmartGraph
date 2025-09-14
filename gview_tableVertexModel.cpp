#include "gview_tableVertexModel.h"

VertexModel::VertexModel(QObject * tata):QAbstractTableModel(tata)
{
    return;
}
QVariant VertexModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return QVariant();
}

QVariant VertexModel::data(const QModelIndex& index,int role) const
{
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole)
    {
        if(index.row()<_vertices_.size())
        {
            if(!_vertices_.at(index.row()))
            {
                return QVariant();
            }
            return _vertices_.at(index.row())->info();
        }
    }
    return QVariant();
}
Qt::ItemFlags VertexModel::flags(const QModelIndex& index) const
{
    if(index.isValid())
    {
        return QAbstractTableModel::flags(index)|Qt::ItemIsEditable|Qt::ItemIsEnabled| Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
    }
    return QAbstractTableModel::flags(index)| Qt::ItemIsDropEnabled;
}
int VertexModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return _vertices_.size();
}
int VertexModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 1;
}
bool VertexModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid()|| index.row()>=_vertices_.size())
    {
        return false;
    }
    if(role==Qt::EditRole)
    {
        if(!_vertices_.at(index.row()))
        {
            return false;
        }
        _vertices_[index.row()]->setInfo(value.toString());
        emit dataChanged(index,index);
        return true;
    }
    return false;
}

QStringList VertexModel::mimeTypes() const
{
    QStringList list;
    list.append("application/x-movedVertex");
    return list;
}
QMimeData* VertexModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData* data = new QMimeData();
    QByteArray encoded;
    QDataStream stream(&encoded,QIODevice::WriteOnly);
    for(const QModelIndex& index: indexes)
    {
        if(index.isValid())
        {
            stream<<index.row();
        }
    }
    data->setData("application/x-movedVertex",encoded);
    return data;
}

bool VertexModel::dropMimeData(const QMimeData* data, Qt::DropAction action,
                  int row, int column, const QModelIndex& parent)
{
    Q_UNUSED(parent)
    Q_UNUSED(column)
    if(action == Qt::IgnoreAction)
    {
        return true;
    }
    if(!data->hasFormat("application/x-movedVertex"))
    {
        return false;
    }
    QByteArray encoded(data->data("application/x-movedVertex"));
    QDataStream stream(&encoded, QIODevice::ReadOnly);
    int insert_row = 0;
    if(row!=-1)
    {
        insert_row = row;
    }
    else if(parent.isValid())
    {
        insert_row = parent.row();
    }
    else
    {
        insert_row = rowCount(QModelIndex());
    }
    int src_row = -1;
    stream>>src_row;
    if(src_row<0 || src_row>=_vertices_.size())
    {
        return false;
    }
    moveRows(QModelIndex(),src_row,1,QModelIndex(),insert_row);
//    beginMoveRows(QModelIndex(),src_row,src_row,
//                  QModelIndex(),insert_row>src_row?insert_row+1:insert_row);
//    std::swap(_vertices_[src_row],_vertices_[insert_row>src_row?insert_row-1:insert_row]);
//    endMoveRows();
    return true;
}

Qt::DropActions VertexModel::supportedDropActions() const
{
    return Qt::MoveAction;
}
Qt::DropActions VertexModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

bool VertexModel::moveRows(const QModelIndex& sourceParent,
              int sourceRow, int count, const
              QModelIndex& destParent, int destChild)
{
    Q_UNUSED(destParent)
    Q_UNUSED(sourceParent)
    if(!count) return false;
    int from = sourceRow>=_vertices_.size()?_vertices_.size()-1:sourceRow;
    int to = destChild>=_vertices_.size()?_vertices_.size()-1:destChild;
    //beginMoveRows(sourceParent,from,from,
    //              destParent,to);
    ///!УВАГА, гэта будзе працаваць толькі для адзіночных пераносаў. Потым трэба будзе
    /// дапрацаваць, каб магчыма было пераносіць некалькі шэрагаў.

    _vertices_.move(from,to);
    //endMoveRows();
    return true;
}

bool VertexModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!count) return false;
    beginInsertRows(parent,row,row+count-1);
    for(int i = row; i<row+count;i++)
    {
        _vertices_.insert(i>_vertices_.size()?_vertices_.size():i,nullptr);
    }
    endInsertRows();
    return true;
}
bool VertexModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if(!count) return false;
    beginRemoveRows(parent,row,row+count-1);
    for(int i = row; i<row+count;i++)
    {
        _vertices_.remove(i>=_vertices_.size()?_vertices_.size()-1:i);
    }
    endRemoveRows();
    return true;
}
void VertexModel::addItem(GViewItem* item, int row)
{
    if(_vertices_.contains(item))
    {
        return;
    }
    if(row<0||row>=_vertices_.size())
    {
        row = _vertices_.size();
    }
    beginInsertRows(QModelIndex(),row,row);
    _vertices_.insert(row,item);
    endInsertRows();
    return;
}

void VertexModel::removeItem(GViewItem* item)
{
    int index = _vertices_.indexOf(item);
    if(index<0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(),index,index);
    _vertices_.remove(index);
    endRemoveRows();
    return;
}

int VertexModel::size()
{
    return _vertices_.size();
}

GViewItem* VertexModel::operator[](int num)
{
    if(num<0 || num>= _vertices_.size())
    {
        return nullptr;
    }
    return _vertices_.at(num);
}

GViewItem* VertexModel::at(int num)
{
    if(num<0 || num>= _vertices_.size())
    {
        return nullptr;
    }
    return _vertices_.at(num);
}

QVector<GViewItem*>::const_iterator VertexModel::begin() const
{
    return _vertices_.cbegin();
}

QVector<GViewItem*>::const_iterator VertexModel::end() const
{
    return _vertices_.cend();
}
QVector<GViewItem*>::const_iterator VertexModel::find(GViewItem* item)
{
    return std::find(begin(),end(),item);
}

bool VertexModel::contains(GViewItem* item)const
{
    return _vertices_.contains(item);
}
