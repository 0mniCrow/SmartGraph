#include "simplegrconnection.h"
#include "abstractGrItem.h"


SimpleGrConnection::SimpleGrConnection(AbstractGrItem* source,
                                       AbstractGrItem* destination,
                                       ItemCommunicator *communicator,
                                       char mode, bool directed,
                                       const item_id_t& id,
                                       qreal weight, QGraphicsObject* tata):
    AbstractGrQtConnection(source,destination,mode, directed, id ,weight, tata)
{
    setCommunicator(communicator);
    return;
}

QRectF SimpleGrConnection::boundingRect() const
{
    if(!checkStatus())
    {
        return QRectF();
    }
    qreal extra = (ABSTRACT_EDGE_WIDTH+getArrowSize())/2.0;
    QRectF b_rect(getStartEndpoint(),getFinishEndpoint());
    b_rect = b_rect.normalized().adjusted(-extra,-extra,extra,extra);
    return b_rect;
}

void SimpleGrConnection::getArrowSholders(double arr_atan, const QPointF& arr_head,
                      QPointF& first_sholder,QPointF& sec_sholder,bool dest_point)
{
    qreal arr_size(getArrowSize());

    double first_point_angle = 0.0;
    double last_point_angle = 0.0;
    if(dest_point)
    {
        first_point_angle = arr_atan-M_PI/3;
        last_point_angle = arr_atan-M_PI+M_PI/3;
    }
    else
    {
        first_point_angle = arr_atan+M_PI/3;
        last_point_angle = arr_atan+M_PI-M_PI/3;
    }

    first_sholder =
            arr_head +
            QPointF(sin(first_point_angle)*arr_size,
                    cos(first_point_angle)*arr_size);

    sec_sholder =
            arr_head +
            QPointF(sin(last_point_angle)*arr_size,
                    cos(last_point_angle)*arr_size);
    return;
}

void SimpleGrConnection::paint(QPainter* painter,
           const QStyleOptionGraphicsItem* option,
           QWidget* widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    if(!checkStatus())
    {
        return;
    };
    QLineF line(getStartEndpoint(),getFinishEndpoint());
    if(qFuzzyCompare(line.length(),qreal(0.0)))
    {
        return;
    }
    char mode = getMode();
    Qt::GlobalColor colour = Qt::white;
    Qt::PenStyle line_type = Qt::NoPen;

    switch(mode)
    {
    case GrEdge_regular:
    {
        colour = Qt::black;
        line_type = Qt::SolidLine;
    }
        break;
    case GrEdge_deletion:
    {
        colour = Qt::darkRed;
        line_type = Qt::SolidLine;
    }
        break;
    case GrEdge_incomplete:
    {
        colour = Qt::black;
        line_type = Qt::DashLine;
    }
        break;
    default:
    {
        qDebug()<<"SimpleGrConnection::paint - impossible state";
    }
    }

    painter->setPen(QPen(colour,
                        CONNECTION_WIDTH,
                        line_type,
                        Qt::RoundCap,
                        Qt::RoundJoin));
    painter->drawLine(line);
    painter->setBrush(Qt::black);
    double angle = std::atan2(-line.dy(),line.dx());

    if(!isDirected() && mode==GrEdge_regular)
    {
        QPointF sourceArrowP1,sourceArrowP2;
        getArrowSholders(angle,getStartEndpoint(),sourceArrowP1,sourceArrowP2, false);
        painter->drawPolygon(QPolygonF()<<
                             line.p1()<<
                             sourceArrowP1<<
                             sourceArrowP2);
    }
    QPointF destArrowP1,destArrowP2;
    getArrowSholders(angle,getFinishEndpoint(),destArrowP1,destArrowP2,true);
    painter->drawPolygon(QPolygonF()<<
                         line.p2()<<
                         destArrowP1<<
                         destArrowP2);
    return;
}

void SimpleGrConnection::redraw()
{
    if(isVisible() && checkStatus())
    {
        update();
    }
    return;
}

QPainterPath SimpleGrConnection::shape() const
{
    if(!getEdgeLength())
    {
        return QPainterPath();
    }
    QPainterPath path(getStartEndpoint());
    path.lineTo(getFinishEndpoint());
    return path;
}

int SimpleGrConnection::type() const
{
    return Type;
}


