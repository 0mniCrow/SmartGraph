#include "abstractgrconnection.h"
#include "abstractGrItem.h"

AbstractGrConnection::AbstractGrConnection(const item_id_t &id, bool directed, QGraphicsObject *tata):
    QGraphicsObject(tata),AbstractGrInterface(id),_src_item_(nullptr),_dest_item_(nullptr),
    _communicator_(nullptr),_directed_(directed),_mode_(GrEdge_Null)
{
    return;
}

void AbstractGrConnection::setArrowSize(qreal ar_size)
{
    if(!_communicator_)
    {
        return;
    }
    _communicator_->setArrowSize(ar_size);
    if(isVisible())
    {
        redraw();
    }
    return;
}

qreal AbstractGrConnection::getArrowSize() const
{
    if(!_communicator_)
    {
        return DEFAULT_ARROW_SIZE;
    }
    return _communicator_->getArrowSize();
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
    if(isVisible())
    {
        redraw();
    }
    return;
}

bool AbstractGrConnection::isDirected() const noexcept
{
    return _directed_;
}

int AbstractGrConnection::type() const
{
    return Type;
}

char AbstractGrConnection::grObjectType() const noexcept
{
    return AbstractConnection;
}

char AbstractGrConnection::grConnectionType() const noexcept
{
    return AbstractConnection;
}

void AbstractGrConnection::setMode(char mode)
{
    _mode_=mode;
    return;
}

char AbstractGrConnection::getMode() const noexcept
{
    return _mode_;
}

void AbstractGrConnection::setCommunicator(ItemCommunicator* communicator)
{
    if(communicator)
    {
        _communicator_= communicator;
    }
    return;
}

bool AbstractGrConnection::hasSourceItem() const noexcept
{
    return _src_item_;
}

bool AbstractGrConnection::hasMainItems() const noexcept
{
    return _src_item_&&_dest_item_;
}

void AbstractGrConnection::setSource(AbstractGrItem* src)
{
    _src_item_ = src;
    if(isVisible())
    {
       redraw();
    }
    return;
}

void AbstractGrConnection::setDestination(AbstractGrItem* dest)
{
    _dest_item_ = dest;
    if(isVisible())
    {
        redraw();
    }
    return;
}

void AbstractGrConnection::setGrX(coord_real x)
{
    Q_UNUSED(x);
    return;
}

void AbstractGrConnection::setGrY(coord_real y)
{
    Q_UNUSED(y);
    return;
}

coord_real AbstractGrConnection::getGrX() const
{
    return x();
}
coord_real AbstractGrConnection::getGrY() const
{
    return y();
}

void AbstractGrConnection::setGrWidth(coord_real width)
{
    Q_UNUSED(width);
}

void AbstractGrConnection::setGrHeight(coord_real height)
{
    Q_UNUSED(height);
}

coord_real AbstractGrConnection::getGrWidth() const
{
    return boundingRect().width();
}

coord_real AbstractGrConnection::getGrHeight() const
{
    return boundingRect().height();
}

void AbstractGrConnection::moveGr(coord_real x, coord_real y)
{
    Q_UNUSED(x) Q_UNUSED(y)
            return;
}

void AbstractGrConnection::drawGr()
{
    redraw();
    return;
}
