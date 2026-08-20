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

}

bool BasicItemManager::deleteConnection(AbstractGrConnection* connection)
{

}
bool BasicItemManager::deleteConnection(uint id)
{

}
AbstractGrConnection* BasicItemManager::findConnection(uint id) const
{

}
AbstractGrConnection* BasicItemManager::findConnection(AbstractGrInterface* source,
                                             AbstractGrInterface* destination)
{

}

QString BasicItemManager::getLastError() const
{

}
