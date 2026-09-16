#include "abstractgrconnection.h"
#include "abstractGrItem.h"

AbstractGrConnection::AbstractGrConnection(const item_id_t &id, bool directed, qreal weight):
    AbstractGrInterface(id),_src_item_(nullptr),_dest_item_(nullptr),
    _directed_(directed),_mode_(GrEdge_Null),_weight_(weight)
{
    return;
}

AbstractGrConnection::AbstractGrConnection(AbstractGrItem* source,
                     AbstractGrItem* destination,
                     char mode, bool directed,
                     const item_id_t& id, qreal weight):AbstractGrInterface(id),
    _src_item_(nullptr),_dest_item_(nullptr),
    _directed_(directed),_mode_(GrEdge_Null),
    _weight_(weight)
{
    switch(mode)
    {
    case GrEdge_regular:
    {
        if(source&&destination)
        {
            setBasicSource(source);
            setBasicDestination(destination);
            _mode_ = mode;
        }
    }
        break;
    case GrEdge_incomplete:
    case GrEdge_deletion:
    {
        if(source)
        {
            setBasicSource(source);
            _mode_=mode;
        }
    }
        break;
    default:{}
    }
    return;
}

bool AbstractGrConnection::checkStatus() const
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

void  AbstractGrConnection::setBasicSource(AbstractGrItem* source)
{
    _src_item_ = source;
    source->addEdge(this);
    return;
}
void  AbstractGrConnection::setBasicDestination(AbstractGrItem* destination)
{
    _dest_item_ = destination;
    destination->addEdge(this);
    return;
}
void AbstractGrConnection::setSource(AbstractGrItem* src)
{
    setBasicSource(src);
    recalcEndpoints();
    redraw();
    return;
}

void AbstractGrConnection::setDestination(AbstractGrItem* dest)
{
    setBasicDestination(dest);
    recalcEndpoints();
    redraw();
    return;
}

void AbstractGrConnection::drawGr()
{
    redraw();
    return;
}
