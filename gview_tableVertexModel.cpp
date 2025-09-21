#include "gview_tableVertexModel.h"

VertexModel::VertexModel(QObject * tata):QAbstractTableModel(tata),
    _vm_flags_(VM_NoFlags),_dragged_row_(INACTIVE_PHANTOM_ROW),
    _phantom_row_(INACTIVE_PHANTOM_ROW)
{
    return;
}

int VertexModel::getActualSize() const
{
    int phantom_shift = _phantom_row_>=0?1:0;
    if(_vm_flags_&VM_Proxy_isActive)
    {
        return _proxy_vector_.size()+phantom_shift;
    }
    return _vertices_.size()+phantom_shift;
}

bool VertexModel::isRowValid(int row) const
{
    return row>=0 && row<getActualSize();
}

QVariant VertexModel::getData(int row) const
{
    if(_vm_flags_&VM_Proxy_isActive)
    {
        if(_proxy_vector_.at(row))
        {
            return _proxy_vector_.at(row)->info();
        }
    }
    else
    {
        if(_vertices_.at(row))
        {
            return _vertices_.at(row)->info();
        }
    }
    return QVariant();
}

bool VertexModel::loadData(int row, const QVariant& data)
{
    if(_vm_flags_&VM_Proxy_isActive)
    {
        if(_proxy_vector_.at(row))
        {
            _proxy_vector_[row]->setInfo(data.toString());
            return true;
        }
    }
    else
    {
        if(_vertices_.at(row))
        {
            _vertices_[row]->setInfo(data.toString());
            return true;
        }
    }
    return false;
}

QVector<GViewItem*>* VertexModel::getActualList()
{
    if(_vm_flags_&VM_Proxy_isActive)
    {
        return &_proxy_vector_;
    }
    return &_vertices_;
}

const QVector<GViewItem*>* VertexModel::getActualCList() const
{
    if(_vm_flags_&VM_Proxy_isActive)
    {
        return &_proxy_vector_;
    }
    return &_vertices_;
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
    ///!todo set selected item in the list selected on the graphic model
    /// and visa versa
    if(!index.isValid())
    {
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole)
    {
        if(index.row()>=getActualSize())
        {
            return QVariant();
        }
        if(_phantom_row_>=0)
        {
            if(_phantom_row_ == index.row()&&
                    _phantom_row_ != _dragged_row_)
            {
                return QString("Drop here...");
            }
            else if(index.row()<_phantom_row_&&
                    index.row()>=_dragged_row_)
            {
                return getData(index.row()+1);
            }
            else if(index.row()>_phantom_row_&&
                    index.row()<=_dragged_row_)
            {
                return getData(index.row()-1);
            }
        }
        return getData(index.row());
    }
    else if(role ==  Qt::BackgroundRole)
    {
        if(_phantom_row_>=0)
        {
        if(_phantom_row_==index.row())
        {
            return QColor(179,237,235);
        }
        else if(index.row()<_phantom_row_&&
                index.row()>=_dragged_row_)
        {
            return QColor(248,255,182);
        }
        else if(index.row()>_phantom_row_&&
                index.row()<=_dragged_row_)
        {
            return QColor(248,255,182);
        }
        }
        return QVariant();
    }
    return QVariant();
}
Qt::ItemFlags VertexModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
    {
        return QAbstractTableModel::flags(index)| Qt::ItemIsDropEnabled;
    }
    if(_phantom_row_>=0 && _phantom_row_==index.row() && _phantom_row_ != _dragged_row_)
    {
        return QAbstractTableModel::flags(index)|Qt::ItemIsEnabled;
    }
    return QAbstractTableModel::flags(index)|
            Qt::ItemIsEditable|Qt::ItemIsEnabled|
            Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

}
int VertexModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return getActualSize()+((_phantom_row_>=0 &&
                             _phantom_row_!=_dragged_row_)?1:0);
}
int VertexModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return VM_COLUMN_COUNT;
}
bool VertexModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!index.isValid()|| index.row()>=getActualSize())
    {
        return false;
    }
    if(role==Qt::EditRole)
    {
        if(_phantom_row_>=0 && _phantom_row_==index.row())
        {
            return false;
        }
        if(loadData(index.row()-(_phantom_row_>index.row()?1:0),value))
        {
            emit dataChanged(index,index);
            return true;
        }
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

int VertexModel::extractMimeData(const QMimeData* mimeData) const
{
    if(!mimeData)
    {
        return -1;
    }
    QByteArray encoded(mimeData->data("application/x-movedVertex"));
    QDataStream stream(&encoded,QIODevice::ReadOnly);
    int src_row = -1;
    stream>>src_row;
    return src_row;
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
    //QByteArray encoded(data->data("application/x-movedVertex"));
    //QDataStream stream(&encoded, QIODevice::ReadOnly);
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
    int src_row = extractMimeData(data);//-1;
    //stream>>src_row;
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
    int from = sourceRow>=getActualList()->size()?
                getActualList()->size()-1:sourceRow;//_vertices_.size()?_vertices_.size()-1:sourceRow;
    int to = destChild>=getActualList()->size()?
                getActualList()->size()-1:destChild;//_vertices_.size()?_vertices_.size()-1:destChild;
    //beginMoveRows(sourceParent,from,from,
    //              destParent,to);
    ///!УВАГА, гэта будзе працаваць толькі для адзіночных пераносаў. Потым трэба будзе
    /// дапрацаваць, каб магчыма было пераносіць некалькі шэрагаў.

    getActualList()->move(from,to);
    //endMoveRows();
    return true;
}

bool VertexModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!count) return false;
    beginInsertRows(parent,row,row+count-1);
    for(int i = row; i<row+count;i++)
    {
        getActualList()->insert(i>getActualList()->size()?getActualList()->size():i,nullptr);
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
        getActualList()->remove((i>=getActualSize())?getActualSize()-1:i);
    }
    endRemoveRows();
    return true;
}
void VertexModel::addItem(GViewItem* item, int row)
{
    if(getActualList()->contains(item))
    {
        return;
    }
    if(row<0||row>=getActualList()->size())
    {
        row = getActualList()->size();
    }
    beginInsertRows(QModelIndex(),row,row);
    getActualList()->insert(row,item);
    endInsertRows();
    return;
}

void VertexModel::removeItem(GViewItem* item)
{

    int index = getActualList()->indexOf(item);
    if(index<0)
    {
        return;
    }
    beginRemoveRows(QModelIndex(),index,index);
    getActualList()->remove(index);
    endRemoveRows();
    return;
}

int VertexModel::size()
{
    return getActualSize();
}

GViewItem* VertexModel::operator[](int num)
{
    if(num<0 || num>= getActualSize())
    {
        return nullptr;
    }
    return getActualList()->at(num);
}

GViewItem* VertexModel::at(int num)
{
    if(num<0 || num>= getActualSize())
    {
        return nullptr;
    }
    return getActualList()->at(num);
}

QVector<GViewItem*>::const_iterator VertexModel::begin() const
{
    return getActualCList()->cbegin();
}

QVector<GViewItem*>::const_iterator VertexModel::end() const
{
    return getActualCList()->cend();
}
QVector<GViewItem*>::const_iterator VertexModel::find(GViewItem* item)
{
    return std::find(begin(),end(),item);
}

bool VertexModel::contains(GViewItem* item)const
{
    return  getActualCList()->contains(item);
}

void VertexModel::setPhantomRow(int phantom_row)
{
    if(_dragged_row_<0 || _phantom_row_ == phantom_row)
    {
        return;
    }
    if(isRowValid(phantom_row))
    {
        if(_phantom_row_>=0)
        {
            //beginRemoveRows(QModelIndex(),_phantom_row_,_phantom_row_);
            _phantom_row_ = INACTIVE_PHANTOM_ROW;
            //endRemoveRows();
        }
        if(phantom_row != _dragged_row_)
        {
            //beginInsertRows(QModelIndex(),phantom_row,phantom_row);
            _phantom_row_ = phantom_row;
            //endInsertRows();
        }

        emit dataChanged(index(phantom_row/*>_dragged_row_?phantom_row-1:phantom_row*/,0),index(getActualSize()-1,0));
    }
    return;
}

int VertexModel::phantomRow() const
{
    return _phantom_row_;
}

void VertexModel::clearPhantomRow()
{
    if(_phantom_row_<0)
    {
        return;
    }
    //beginRemoveRows(QModelIndex(),_phantom_row_,_phantom_row_);
    int temp_ph_row = _phantom_row_;
    _phantom_row_ = INACTIVE_PHANTOM_ROW;
    //endRemoveRows();
    emit dataChanged(index(temp_ph_row,0),index(getActualSize()-1,0));
    return;
}

void VertexModel::setDraggedRow(int dragged_row)
{
    if(isRowValid(dragged_row))
    {
        _dragged_row_ = dragged_row;
    }
    return;
}
void VertexModel::clearDraggedRow()
{
    _dragged_row_ = INACTIVE_PHANTOM_ROW;
    return;
}

void VertexModel::setVMFlags(char vm_flags)
{
    _vm_flags_ = vm_flags;
    return;
}
char VertexModel::getVMFlags() const
{
    return _vm_flags_;
}

bool VertexModel::sortVM(GViewItem* left, GViewItem* right)
{
    return left->info()>right->info();
}

bool VertexModel::filter(GViewItem* element)
{
    return !element->info().isEmpty();
}
