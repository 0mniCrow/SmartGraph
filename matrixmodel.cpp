#include "matrixmodel.h"

MatrixModel::MatrixModel(QObject *tata):QAbstractTableModel(tata),
    #ifdef INTEGER_MATRIX
    _visual_data_(DEFAULT_ROW_COUNT,vector<int>(DEFAULT_COL_COUNT,0)),
    #elif defined(CHAR_MATRIX)
    _visual_data_(DEFAULT_ROW_COUNT,vector<char>(DEFAULT_COL_COUNT,' ')),
    #endif
    _actions_(0), _cur_action_(_actions_.end()),_play_flag_(true)
{
    return;
}

MatrixModel::MatrixModel(int rows, int cols, QObject* tata):QAbstractTableModel(tata),
    #ifdef INTEGER_MATRIX
    _visual_data_(rows,vector<int>(cols,0)),
    #elif defined(CHAR_MATRIX)
    _visual_data_(rows,vector<char>(cols,' ')),
    #endif
    _actions_(0), _cur_action_(_actions_.end()),
    _play_flag_(true)
{
    return;
}

MatrixModel::MatrixModel(
        #ifdef INTEGER_MATRIX
        const vector<vector<int>>& model,
        #elif defined(CHAR_MATRIX)
        const vector<vector<char>>& model,
        #endif
        QObject* tata):
    QAbstractTableModel(tata),_visual_data_(model),_actions_(0),
    _cur_action_(_actions_.end()),_play_flag_(true)
{
    return;
}

bool MatrixModel::chBorderSize(int row, int col) const
{
    int row_size = static_cast<int>(_visual_data_.size());
    if(row_size)
    {
        int col_size = static_cast<int>(_visual_data_.at(0).size());
        if((0<=row)&&(row<row_size)&&(0<=col)&&(col<col_size))
        {
            return true;
        }
    }
    return false;
}

QVariant MatrixModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    Q_UNUSED(role)
    return section;
}

QVariant MatrixModel::data(const QModelIndex& index,int role) const
{
    if(!chBorderSize(index.row(),index.column()))
    {
        return QVariant();
    }
    if(role==Qt::DisplayRole||role==Qt::EditRole)
    {
        if(_cur_action_!=_actions_.end())
        {
            if(_play_flag_)
            {
                if((_cur_action_->row==index.row())&&
                        (_cur_action_->column==index.column()))
                {
                    return QString(_cur_action_->value.c_str());
                }
            }
        }
#ifdef INTEGER_MATRIX
        int answer = _visual_data_.at(index.row()).at(index.column());
#elif defined(CHAR_MATRIX)
        QString answer(_visual_data_.at(index.row()).at(index.column()));
#endif
        return QVariant(answer);

    }
    else if(role==Qt::BackgroundRole)
    {
        if(_cur_action_!=_actions_.end())
        {
            if(_play_flag_)
            {
                if((_cur_action_->row==index.row())&&
                        (_cur_action_->column==index.column()))
                {
                    return QColor(180,249,196);
                }
            }
        }
    }
    return QVariant();
}

Qt::ItemFlags MatrixModel::flags(const QModelIndex& index) const
{
    if(index.isValid())
    {
        return QAbstractTableModel::flags(index)|Qt::ItemIsEditable|Qt::ItemIsEnabled;
    }
    return QAbstractTableModel::flags(index);
}

int MatrixModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(_visual_data_.size());
}

int MatrixModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if(rowCount())
    {
        return static_cast<int>(_visual_data_.at(0).size());
    }
    return 0;
}

bool MatrixModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if(!chBorderSize(index.row(),index.column()))
    {
        return false;
    }
    if(role==Qt::EditRole)
    {
        if(value.isValid()&&value.canConvert<int>())
        {
#ifdef INTEGER_MATRIX
            _visual_data_.at(index.row()).at(index.column()) = value.toInt();
#elif defined(CHAR_MATRIX)
            _visual_data_.at(index.row()).at(index.column()) = value.toString().toLatin1().at(0);
#endif
            emit dataChanged(index,index);
            return true;
        }
    }
    return false;
}

bool MatrixModel::insertRows(int row, int count, const QModelIndex& parent)
{
    if(!count)
    {
        return false;
    }
    beginInsertRows(parent,row,row+count-1);
    while(count)
    {
        #ifdef INTEGER_MATRIX
        vector<int> temp_vector;
        int def_val = 0;
        #elif defined(CHAR_MATRIX)
        vector<char> temp_vector;
        char def_val = ' ';
        #endif
        if(!rowCount())
        {
            temp_vector.resize(1,def_val);
            _visual_data_.push_back(temp_vector);
        }
        else if(row<rowCount())
        {
            temp_vector.resize(columnCount(),def_val);
            _visual_data_.insert(_visual_data_.begin()+row,temp_vector);
        }
        else
        {
            temp_vector.resize(columnCount(),def_val);
            _visual_data_.push_back(temp_vector);
        }
        row++;
        count--;
    }
    endInsertRows();
    return true;
}

bool MatrixModel::insertColumns(int column, int count, const QModelIndex& parent)
{
    if((!count)||(!rowCount()))
    {
        return false;
    }
    beginInsertColumns(parent,column,column+count-1);
    while(count)
    {

        #ifdef INTEGER_MATRIX
        vector<vector<int>>::iterator it = _visual_data_.begin();
        int def_val = 0;
        #elif defined(CHAR_MATRIX)
        vector<vector<char>>::iterator it = _visual_data_.begin();
        char def_val = ' ';
        #endif
        if(!columnCount())
        {
//            for(vector<int>& row:_visual_data_)
//            {
//                row.push_back(0);
//            }
            while(it!=_visual_data_.end())
            {
                it->push_back(def_val);
                it++;
            }
        }
        else if(column<columnCount())
        {
//            for(vector<int>& row:_visual_data_)
//            {
//                row.insert(row.begin()+column,0);
//            }
            while(it!=_visual_data_.end())
            {
                it->insert(it->begin()+column,def_val);
                it++;
            }
        }
        else
        {
//            for(vector<int>& row:_visual_data_)
//            {
//                row.push_back(0);
//            }
            while(it!=_visual_data_.end())
            {
                it->push_back(def_val);
                it++;
            }
        }
        column++;
        count--;
    }
    endInsertColumns();
    return true;
}

bool MatrixModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if((!count)||(row+count>rowCount()))
    {
        return false;
    }
    beginRemoveRows(parent,row,row+count-1);
    while(count)
    {
        if(!rowCount())
        {
            break;
        }
        else if(row<rowCount())
        {
            _visual_data_.erase(_visual_data_.begin()+row);
        }
        else
        {
            _visual_data_.pop_back();
        }
        row++;
        count--;
    }
    endRemoveRows();
    return true;
}

bool MatrixModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    if((!count)||(!rowCount())||(column+count>columnCount()))
    {
        return false;
    }
    beginRemoveColumns(parent,column,column+count-1);
    while(count)
    {
#ifdef INTEGER_MATRIX
        vector<vector<int>>::iterator it = _visual_data_.begin();
#elif defined(CHAR_MATRIX)
        vector<vector<char>>::iterator it = _visual_data_.begin();
#endif
        if(!columnCount())
        {
            break;
        }
        else if(column<columnCount())
        {
//            for(vector<int>& row:_visual_data_)
//            {
//                row.erase(row.begin()+column);
//            }
            while(it!=_visual_data_.end())
            {
                it->erase(it->begin()+column);
                it++;
            }
        }
        else
        {
//            for(vector<int>& row:_visual_data_)
//            {
//                row.pop_back();
//            }
            while(it!=_visual_data_.end())
            {
                it->pop_back();
                it++;
            }
        }
    }
    endRemoveColumns();
    return true;
}

bool MatrixModel::setData(
        #ifdef INTEGER_MATRIX
        const vector<vector<int>>& data
        #elif defined(CHAR_MATRIX)
        const vector<vector<char>>& data
        #endif
        )
{
    beginResetModel();
    _visual_data_ = data;
    endResetModel();
    return true;
}

bool MatrixModel::setActions(const vector<PlayAction>& actions)
{
    _actions_ = actions;
    _cur_action_= _actions_.end();
    return true;
}

void MatrixModel::startActions()
{
    if(!_actions_.size())
    {
        return;
    }
    if(_cur_action_==_actions_.end())
    {
        _cur_action_ = _actions_.begin();
    }
    _next_action_ = _cur_action_;
    play();
    return;
}

bool MatrixModel::play()
{
    if(!_actions_.size())
    {
        return false;
    }
    if(_play_flag_)
    {
        QModelIndex prew_index = this->index(_cur_action_->row,_cur_action_->column);
        _cur_action_ = _next_action_;
        emit dataChanged(prew_index,prew_index);
        if(_cur_action_==_actions_.end())
        {
            return true;
        }
        else
        {
            QModelIndex cur_index = this->index(_cur_action_->row,_cur_action_->column);
            emit dataChanged(cur_index,cur_index);
            int dist = std::distance(_actions_.begin(),_cur_action_);
            int whole = static_cast<int>(_actions_.size())-1;
            emit updateBar(dist,whole);
            _next_action_++;
            QTimer * timer = new QTimer(this);
            connect(timer,&QTimer::timeout,this,&MatrixModel::play);
            connect(timer,&QTimer::timeout,timer,&QTimer::deleteLater);
            timer->start(DEFAULT_TIMER);
        }
    }
    return true;
}
void MatrixModel::changePlayFlag(bool play_flag)
{
    _play_flag_ = play_flag;
}
