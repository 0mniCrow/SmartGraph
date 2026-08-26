#include "basicitemmanager.h"

BasicItemManager::BasicItemManager():AbstractItemManager()
{
    return;
}
BasicItemManager::~BasicItemManager()
{
        ///!TODO: выдаліць усе элементы
}

bool BasicItemManager::checkItemType(char gr_type) const
{
    if(gr_type!=StaticGrItem::StaticItem)
    {
        return false;
    }
    return true;
}
bool BasicItemManager::checkConnectionType(char gr_type) const
{
    if(gr_type!=SimpleGrConnection::SimpleConnection)
    {
        return false;
    }
    return true;
}

AbstractGrInterface* BasicItemManager::createItem(qreal x_coord, qreal y_coord, char type, uint id)
{
    if(!checkItemType(type))
    {
        return nullptr;
    }
    if(!id)
    {
        id = generateID();
    }
    else
    {
        if(isExist(id))
        {
            return nullptr;
        }
        addID(id);
    }

    StaticGrItem* new_item = nullptr;

    switch(type)
    {
    case StaticGrItem::StaticItem:
    {
         new_item = new StaticGrItem(id);
    }
        break;
    default:
    {
        removeID(id);
        return nullptr;
    }
        break;
    }
    new_item->moveGr(x_coord,y_coord);
    _items_.insert(id,new_item);
    return new_item;
}

bool BasicItemManager::deleteItem(AbstractGrInterface* item)
{
    if(!item)
    {
        return false;
    }
    QMap<uint,AbstractGrItem*>::const_iterator it = _items_.find(item->getGrID());
    if(it==_items_.cend())
    {
        return false;
    }
    emit objectRemoved(item->getGrID());
    _items_.erase(it);
    AbstractGrItem* gr_item = dynamic_cast<AbstractGrItem*>(item);
    if(gr_item)
    {
        gr_item->deleteLater();
    }
    else
    {
        delete item;
    }
    return true;
}

bool BasicItemManager::deleteItem(uint id)
{
    QMap<uint,AbstractGrItem*>::const_iterator it = _items_.find(id);
    if(it==_items_.cend())
    {
        return false;
    }
    return deleteItem(it.value());
}

AbstractGrInterface* BasicItemManager::findItem(uint id) const
{
    QMap<uint,AbstractGrItem*>::const_iterator it = _items_.find(id);
    if(it==_items_.cend())
    {
        return nullptr;
    }
    return it.value();
}

AbstractGrInterface* BasicItemManager::findItem(qreal x_coord, qreal y_coord)
{
    QMap<uint,AbstractGrItem*>::const_iterator it = _items_.cbegin();
    while(it!=_items_.cend())
    {
        AbstractGrItem* gr_item = dynamic_cast<AbstractGrItem*>(it.value());
        if(gr_item)
        {
            QPointF gr_point = gr_item->mapFromScene(x_coord,y_coord);
            QRectF gr_rect = gr_item->boundingRect();
            if(gr_rect.contains(gr_point))
            {
                return gr_item;
            }
        }
        ++it;
    }
    return nullptr;
}

AbstractGrConnection* BasicItemManager::createConnection(AbstractGrInterface* source,
                                               AbstractGrInterface* destination,
                                               char type,
                                                         uint id)
{
    switch(type)
    {
    case AbstractGrConnection::GrEdge_regular:
    {
        if(!destination)
        {
            return nullptr;
        }
    }
    [[fallthrough]];
    case AbstractGrConnection::GrEdge_incomplete:
    case AbstractGrConnection::GrEdge_deletion:
    {
        if(!source)
        {
            return nullptr;
        }
    }
        break;
    default:
    {

    }
    }
    if(!id)
    {
        id = generateID();
    }
    else
    {
        if(isExist(id))
        {
            return nullptr;
        }
        addID(id);
    }
    SimpleGrConnection* new_conn = new SimpleGrConnection(dynamic_cast<AbstractGrItem*>(source),
                                                          dynamic_cast<AbstractGrItem*>(destination),
                                                          nullptr,id);
    _connections_.insert(id,new_conn);
    return new_conn;
}

bool BasicItemManager::deleteConnection(AbstractGrConnection* connection)
{
    if(!connection)
    {
        return false;
    }
    QMap<uint,AbstractGrConnection*>::iterator it = _connections_.find(connection->getGrID());
    if(it==_connections_.end())
    {
        return false;
    }
    AbstractGrItem* item = (*it)->getSource();
    item->delEdge(*it);
    if(item=(*it)->getDestination())
    {
        item->delEdge(*it);
    }
    (*it)->deleteLater();
    return true;
}

bool BasicItemManager::deleteConnection(uint id)
{
    QMap<uint,AbstractGrConnection*>::iterator it = _connections_.find(id);
    if(it==_connections_.end())
    {
        return false;
    }
    return deleteConnection(it.value());
}

AbstractGrConnection* BasicItemManager::findConnection(uint id) const
{
    QMap<uint,AbstractGrConnection*>::const_iterator it = _connections_.find(id);
    if(it==_connections_.cend())
    {
        return nullptr;
    }
    return it.value();
}

AbstractGrConnection* BasicItemManager::findConnection(AbstractGrInterface* source,
                                             AbstractGrInterface* destination)
{
    QMap<uint,AbstractGrConnection*>::iterator it =std::find_if(_connections_.begin(),_connections_.end(),[source,destination](AbstractGrConnection* conn)
    {
        if((conn->getSource()==source)&&(conn->getDestination()==destination))
        {
            return true;
        }
        return false;
    });
    if(it==_connections_.end())
    {
        return nullptr;
    }
    return it.value();
}

QStringView BasicItemManager::getLastError() const
{
    return _last_error_;
}

void BasicItemManager::addCallbackFunc(const QString& model_name,
                                       std::function<void(unsigned int)> callback_func)
{
    if(_callbacks_.contains(model_name))
    {
        _callbacks_[model_name] = callback_func;
    }
    else
    {
        _callbacks_.insert(model_name,callback_func);
    }
    return;
}
void BasicItemManager::removeCallbackFunc(const QString& model_name)
{
    _callbacks_.remove(model_name);
    return;
}
