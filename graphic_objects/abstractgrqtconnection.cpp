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

qreal AbstractGrQtConnection::getLength(const QPointF& src, const QPointF& dest) const
{
    QPointF delta(src - dest);
    return std::hypot(delta.x(),delta.y());
}

void AbstractGrQtConnection::recalculation(const QPointF& start_point, const QPointF& fin_point)
{
    qreal length = getLength(start_point, fin_point);
    prepareGeometryChange();
    char mode = getMode();
    bool long_enough = true;

    AbstractGrQtItem* src_item = dynamic_cast<AbstractGrQtItem*>(getSource());
    AbstractGrQtItem* dest_item = nullptr;
    if(!src_item)
    {
        return;
    }
    qreal src_radius  = src_item->getRadius();
    qreal dest_radius = 0.0;
    if(length<=src_radius)
    {
        long_enough = false;
    }
    if(mode == GrEdge_regular)
    {
        dest_item =dynamic_cast<AbstractGrQtItem*>(getDestination());
        if(!dest_item)
        {
            return;
        }
        dest_radius = dest_item->getRadius();
        if(length<=(src_radius+dest_radius))
        {
            long_enough = false;
        }
    }
    if(long_enough)
    {
        QPointF delta = start_point-fin_point;
        QPointF start_offset((delta.x()*src_radius)/length,
                            (delta.y()*src_radius)/length);
        setStartEndpoint(start_point+start_offset);
        if(mode == GrEdge_regular)
        {
            QPointF end_offset((delta.x()*dest_radius)/length,
                               (delta.y()*dest_radius)/length);
            setFinishEndpoint(fin_point-end_offset);
        }
        else
        {
            setFinishEndpoint(fin_point);
        }
    }
    else
    {
        setStartEndpoint(start_point);
        setFinishEndpoint(start_point);
    }
    return;
}

void AbstractGrQtConnection::recalcEndpoints()
{
    if(!checkStatus())
    {
        return;
    }
    AbstractGrQtItem* src_item = dynamic_cast<AbstractGrQtItem*>(getSource());
    AbstractGrQtItem* dest_item = dynamic_cast<AbstractGrQtItem*>(getDestination());
    if(src_item && dest_item)
    {
        recalculation(mapFromItem(src_item,0.0,0.0),mapFromItem(dest_item,0.0,0.0));
    }
    return;
}

void AbstractGrQtConnection::recalcEndpoints(const QPointF& destination)
{
    if(destination.isNull())
    {
        return;
    }
    if(!checkStatus())
    {
        return;
    }
    AbstractGrQtItem* src_item = dynamic_cast<AbstractGrQtItem*>(getSource());
    if(src_item)
    {
        recalculation(mapFromItem(src_item,0.0,0.0),mapFromScene(destination));
    }
    return;
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



qreal AbstractGrQtConnection::getLength() const
{
     if(getMode()!=GrEdge_regular)
     {
         return 0.0;
     }

     AbstractGrQtItem* src_item = dynamic_cast<AbstractGrQtItem*>(getSource());
     AbstractGrQtItem* dest_item = dynamic_cast<AbstractGrQtItem*>(getDestination());
     if(src_item && dest_item)
     {
         return getLength(mapFromItem(src_item,0.0,0.0),mapFromItem(dest_item,0.0,0.0));
     }
     return 0.0;
}

qreal AbstractGrQtConnection::getGrStartX() const
{
    return _start_endpoint_.x();
}
qreal AbstractGrQtConnection::getGrEndX() const
{
    return _finish_endpoint_.x();
}
qreal AbstractGrQtConnection::getGrStartY() const
{
    return _start_endpoint_.y();
}
qreal AbstractGrQtConnection::getGrEndY() const
{
    return _finish_endpoint_.y();
}

void AbstractGrQtConnection::getGrStartCoords(qreal& x, qreal& y) const
{
    x = _start_endpoint_.x();
    y = _start_endpoint_.y();
    return;
}
void AbstractGrQtConnection::getGrEndCoords(qreal& x, qreal& y) const
{
    x = _finish_endpoint_.x();
    y = _finish_endpoint_.y();
    return;
}
