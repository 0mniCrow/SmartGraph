#include "abstractGrItem.h"
#include "graphic_objects/abstractgrconnection.h"
#include "qgraphicsview.h"
AbstractGrItem::AbstractGrItem(const item_id_t &id):
    AbstractGrInterface(id)
{
    return;
}

void AbstractGrItem::addEdge(AbstractGrConnection* edge)
{
    _edges_.append(edge);
    edge->recalculate();
    return;
}

void AbstractGrItem::delEdge(AbstractGrConnection* edge)
{
    auto it = std::find(_edges_.cbegin(),_edges_.cend(),edge);
    if(it!= _edges_.cend())
    {
        _edges_.erase(it);
    }
    return;
}

QList<AbstractGrConnection*> AbstractGrItem::getEdges() const
{
    QList<AbstractGrConnection*> list(_edges_);
    list.detach();
    return list;
}
