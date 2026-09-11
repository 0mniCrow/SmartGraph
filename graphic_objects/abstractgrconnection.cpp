#include "abstractgrconnection.h"
#include "abstractGrItem.h"

AbstractGrConnection::AbstractGrConnection(const item_id_t &id, bool directed):
    AbstractGrInterface(id),_src_item_(nullptr),_dest_item_(nullptr),
    _directed_(directed),_mode_(GrEdge_Null)
{
    return;
}

bool AbstractGrConnection::checkStatus()
{
    switch(getMode())
    {
    case GrEdge_regular:
    {
        if(!getDestination())
        {
            return false;
        }
    }
    [[fallthrough]];
    case GrEdge_incomplete:
    case GrEdge_deletion:
    {
        if((!getSource()))
        {
            return false;
        }
    }
        break;
    default:
    {
        return false;
    }
    }
    return true;
}

AbstractGrItem* AbstractGrConnection::getSource() const noexcept
{
    return _src_item_;
}

AbstractGrItem* AbstractGrConnection::getDestination() const noexcept
{
    return _dest_item_;
}

void AbstractGrConnection::setDirected(bool state)
{
    _directed_ = state;
    redraw();
    return;
}

bool AbstractGrConnection::isDirected() const noexcept
{
    return _directed_;
}

void AbstractGrConnection::setWeight(qreal weight)
{
    _weight_ = weight;
    redraw();
    return;
}

qreal AbstractGrConnection::getWeight() const noexcept
{
    return _weight_;
}

void AbstractGrConnection::setLengthAsWeigth()
{
    _weight_ = getLength();
    return;
}

char AbstractGrConnection::grObjectType() const noexcept
{
    return AbstractConnection;
}

void AbstractGrConnection::setMode(char mode)
{
    _mode_=mode;
    redraw();
    return;
}

char AbstractGrConnection::getMode() const
{
    return _mode_;
}

void AbstractGrConnection::setSource(AbstractGrItem* src)
{
    _src_item_ = src;
    src->addEdge(this);
    recalcEndpoints();
    redraw();
    return;
}

void AbstractGrConnection::setDestination(AbstractGrItem* dest)
{
    _dest_item_ = dest;
    dest->addEdge(this);
    recalcEndpoints();
    redraw();
    return;
}

void AbstractGrConnection::drawGr()
{
    redraw();
    return;
}
