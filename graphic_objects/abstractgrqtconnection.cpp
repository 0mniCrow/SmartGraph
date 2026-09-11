#include "abstractgrqtconnection.h"
#include "graphic_objects/abstractgrqtitem.h"

AbstractGrQtConnection::AbstractGrQtConnection(const item_id_t &id,
                                               bool directed,
                                               QGraphicsObject *tata):
    QGraphicsObject(tata),AbstractGrConnection(id,directed)
{
    setAcceptedMouseButtons(Qt::NoButton);
    setZValue(-1);
    return;
}

QPointF AbstractGrQtConnection::getSourcePoint() const
{
    AbstractGrQtItem* QtGrItem = dynamic_cast<AbstractGrQtItem*>(getSource());
    return mapFromItem(QtGrItem,0,0);
}

QPointF AbstractGrQtConnection::getDestinationPoint() const
{
    AbstractGrQtItem* QtGrItem = dynamic_cast<AbstractGrQtItem*>(getDestination());
    return mapFromItem(QtGrItem,0,0);
}

void AbstractGrQtConnection::redraw()
{
    update();
    return;
}

bool AbstractGrQtConnection::checkStatus()
{
    if(!_communicator_)
    {
        return false;
    }
    return AbstractGrConnection::checkStatus();
}

void AbstractGrQtConnection::recalcEndpoints()
{

}

void AbstractGrQtConnection::setArrowSize(qreal ar_size)
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

AbstractGrQtConnection::~AbstractGrQtConnection()
{
    emit connectionAboutToBeDestroyed(getGrID());
    return;
}

qreal AbstractGrQtConnection::getArrowSize() const
{
    if(!_communicator_)
    {
        return DEFAULT_ARROW_SIZE;
    }
    return _communicator_->getArrowSize();
}

void AbstractGrQtConnection::setCommunicator(ItemCommunicator* communicator)
{
    if(communicator)
    {
        _communicator_= communicator;
    }
    return;
}

ItemCommunicator* AbstractGrQtConnection::getCommunicator() const noexcept
{
    return _communicator_;
}


void AbstractGrQtConnection::setStartEndpoint(const QPointF& point)
{
    _start_endpoint_ = point;
    redraw();
    return;
}

QPointF AbstractGrQtConnection::getStartEndpoint() const
{
    return _start_endpoint_;
}

void AbstractGrQtConnection::setFinishEndpoint(const QPointF& point)
{
    _finish_endpoint_ = point;
    redraw();
    return;
}
QPointF AbstractGrQtConnection::getFinishEndpoint() const
{
    return _finish_endpoint_;
}
