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

}

bool BasicItemManager::deleteItem(uint id)
{

}
AbstractGrInterface* BasicItemManager::findItem(uint id) const
{

}
AbstractGrInterface* BasicItemManager::findItem(qreal x_coord, qreal y_coord)
{

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
