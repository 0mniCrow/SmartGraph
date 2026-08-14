#include "abstractitemmanager.h"

AbstractItemManager::AbstractItemManager()
{
    _accumulator_=0;
}

uint AbstractItemManager::generateID()
{
    while(true)
    {
        if(!isExist(++_accumulator_))
        {
            _IDs_.insert(_accumulator_);
            break;
        }
        if(_accumulator_==UINT_MAX)
        {
            _accumulator_=0;
        }
    }
    return _accumulator_;
}

bool AbstractItemManager::isExist(uint id)
{
    return _IDs_.contains(id);
}

bool AbstractItemManager::addID(uint id)
{
    if(_IDs_.contains(id))
    {
        return false;
    }
    _IDs_.insert(id);
    return true;
}

bool AbstractItemManager::removeID(uint id)
{
    return _IDs_.remove(id);
}
