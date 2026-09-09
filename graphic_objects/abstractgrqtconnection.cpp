#include "abstractgrqtconnection.h"

AbstractGrQtConnection::AbstractGrQtConnection(const item_id_t &id,
                                               bool directed,
                                               QGraphicsObject *tata):
    QGraphicsObject(tata),AbstractGrConnection(id,directed)
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

