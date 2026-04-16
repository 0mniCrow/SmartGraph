#include "gviewport.h"
/*
 * //todo:
 * Дадаць: сутыкненьне вяршын,
 * стварэньне іконак.
 * Дадаць да вяршын трыманьне відарысаў.
 * Дадаць магчымасьць адсякаць квадрат іконкі.
 *
 *
 * (done) Зрабіць: сьпіс усіх створаных вяршын.
 * (done)Магчыма перацягваць вяршыны,
 *          каб ставіць адну за адной, сартыроўка.
*/
GViewPort::GViewPort(int vertex_radius, VertexModel *model, QWidget *tata):
    QGraphicsView(tata),
    _vertices_(model),
    _vertex_radius_(vertex_radius),
    _controls_state_(0),_counter_(0)
{
    _new_edge_=  nullptr;
    _del_edge_=  nullptr;
    _selected_vertex_= nullptr;
    setMouseTracking(true);
    _no_image_ = QPixmap(":/res/icons/icons/no_image3.svg").
            scaled(_vertex_radius_*2,_vertex_radius_*2,Qt::KeepAspectRatio);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);

    //setDragMode(QGraphicsView::ScrollHandDrag);
    return;
}

void GViewPort::clear()
{
    QList<GViewItem*> items;

    for(int i = 0; i<_vertices_->size();i++)
    {
        items.push_back(_vertices_->at(i));
    }
    for(GViewItem* item:items)
    {
        setMode(GPort_delete);
        deleteItem(item);
    }
}

QGraphicsItem* GViewPort::grabItem(QMouseEvent* m_event)
{
    return scene()->itemAt(mapToScene(m_event->pos()),transform());
}

GViewItem* GViewPort::grabGItem(QMouseEvent* m_event)
{
    QGraphicsItem * g_item = grabItem(m_event);
    if(g_item)
    {
        GViewItem* item = qgraphicsitem_cast<GViewItem*>(g_item);
        if(item)
        {
            return item;
        }
    }
    return nullptr;
}

void GViewPort::addItem(GViewItem *vertex, const QPoint &pos)
{
    if(_mode_!= GPort_add || !vertex)
    {
        return;
    }
    if(_vertices_->contains(vertex))//if(_vertices_.contains(vertex))
    {
        return;
    }
    _vertices_->addItem(vertex);//_vertices_.push_back(vertex);
    scene()->addItem(vertex);
    vertex->setPos(mapToScene(pos));
    setMode(GPort_NoMode);
    return;
}

void GViewPort::deleteItem(GViewItem* vertex)
{
    if(_mode_!= GPort_delete ||
            !vertex)
    {
        return;
    }
    if(vertex->isSelected())    //Калі вертэкс быў выбраны падначаленым механізмам
    {                           //выбар здымаецца на ўзроўні элемента.
        vertex->setSelected(false);
        //_selected_vertex_ = nullptr;
    }
    if(vertex==_selected_vertex_) //Калі вертэкс быў выбраны карыстальнікам
    {                             //спасылка на элемент выдаляецца са сховішча
        _selected_vertex_=nullptr;
    }

    auto it = _vertices_->find(vertex);
    //QList<GViewItem*>::const_iterator it = std::find(_vertices_.cbegin(),_vertices_.cend(),vertex);
    if(it!=_vertices_->end())
    {

        delLinkedEdges(vertex);
        scene()->removeItem(vertex);
//        if(*it == _selected_vertex_)
//        {
//            _selected_vertex_ = nullptr;
//        }

        _vertices_->removeItem(vertex);
        delete vertex;
        setMode(GPort_NoMode);
    }
    if(_selected_vertex_)
    {
        selectItem(_selected_vertex_,true);
    }
    return;
}

void GViewPort::delLinkedEdges(GViewItem*vertex)
{
    if(!vertex)
    {
        return;
    }
    QList<GViewEdge*>::iterator it = _edges_.begin();
    while(it!=_edges_.end())
    {
        if((*it)->source()==vertex||(*it)->destination()==vertex)
        {
            (*it)->source()->delEdge(*it);
            (*it)->destination()->delEdge(*it);
            scene()->removeItem(*it);
            delete *it;
            it = _edges_.erase(it);

        }
        else
        {
            it++;
        }
    }
    return;
}

bool GViewPort::addEdge(GViewItem* source, GViewItem* dest, bool directed)
{
    if(!source || !dest)
    {
        return false;
    }

    for(GViewEdge* edge: _edges_)
    {
        if(edge->source()==source&&edge->destination()==dest)
        {
            return false;
        }
        else if(edge->destination()==source&&edge->source()==dest)
        {
            if(edge->isDirected())
            {
                edge->setDirected(false);
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    GViewEdge * new_edge = new GViewEdge(source,dest,_vertex_radius_,directed);
    source->addEdge(new_edge);
    dest->addEdge(new_edge);
    scene()->addItem(new_edge);
    _edges_.push_back(new_edge);
    return true;
}

void GViewPort::startAddEdge(GViewItem* src)
{
    if(_mode_ != GPort_startAddEdge ||
            _new_edge_|| !src)
    {
        return;
    }
    _new_edge_ = new GViewEdge(src,_vertex_radius_,true);
    scene()->addItem(_new_edge_);
    //setMouseTracking(true);
    _mode_=GPort_finAddEdge;
    return;
}

void GViewPort::finishAddEdge(GViewItem* dest)
{

    if(_mode_!= GPort_finAddEdge ||
            !_new_edge_ || !dest)
    {
        return;
    }
    if(_new_edge_->source()==dest)
    {
        return;
    }
    //setMouseTracking(false);

    for(GViewEdge* edge:_edges_)
    {
        if(edge->source()==_new_edge_->source() &&
                edge->destination() == dest)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
            setMode(GPort_NoMode);
            return;
        }
        else if(edge->destination()==_new_edge_->source()&&
                edge->source()==dest)
        {
            if(edge->isDirected())
            {
                edge->setDirected(false);
                //edge->destination()->addEdge(edge);
            }
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
            setMode(GPort_NoMode);
            return;
        }
    }
    _new_edge_->source()->addEdge(_new_edge_);
    _new_edge_->setDest(dest);
    _new_edge_->destination()->addEdge(_new_edge_);
    _edges_.push_back(_new_edge_);
    _new_edge_ = nullptr;
    setMode(GPort_NoMode);
    return;
}

void GViewPort::startDelEdge(GViewItem* src)
{
    if(_mode_ != GPort_startDelEdge ||
            _del_edge_||
            _new_edge_||
            !src)
    {
        return;
    }
    _del_edge_ = src;
    _new_edge_ = new GViewEdge(src,_vertex_radius_,true,GViewEdge::GVedge_deletion);
    scene()->addItem(_new_edge_);
    _mode_=GPort_finDelEdge;
    //setMouseTracking(true);
    return;
}

void GViewPort::finishDelEdge(GViewItem* dest)
{
    if(_mode_ != GPort_finDelEdge ||
            !_del_edge_||
            !_new_edge_||
            !dest)
    {
        return;
    }
    QList<GViewEdge*>::const_iterator it = _edges_.constBegin();
    while(it!=_edges_.constEnd())
    {
        if((*it)->source()==_del_edge_ && (*it)->destination()==dest)
        {
            break;
        }
        else if((*it)->source()==dest &&
                (*it)->destination()==_del_edge_ &&
                !(*it)->isDirected())
        {
            break;
        }
        it++;
    }
    if(it!=_edges_.constEnd())
    {
        (*it)->source()->delEdge(*it);
        (*it)->destination()->delEdge(*it);
        if(!(*it)->isDirected())
        {
            (*it)->destination()->delEdge(*it);
        }
        scene()->removeItem(*it);
        delete *it;
        _edges_.erase(it);
        _del_edge_ = nullptr;
        scene()->removeItem(_new_edge_);
        delete _new_edge_;
        _new_edge_=nullptr;
        //setMouseTracking(false);
        setMode(GPort_NoMode);
    }
    return;
}

void GViewPort::setMode(GPort_Mode mode)
{
    switch(_mode_)
    {
    case GPort_add:
    case GPort_delete:
    case GPort_startAddEdge:
    {
        QApplication::restoreOverrideCursor();
    }
        break;
    case GPort_finAddEdge:
    {
        QApplication::restoreOverrideCursor();
        if(_new_edge_)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
        }
    }
        break;
    case GPort_finDelEdge:
    {
        if(_new_edge_)
        {
            scene()->removeItem(_new_edge_);
            delete _new_edge_;
            _new_edge_ = nullptr;
        }
        if(_del_edge_)
        {
            _del_edge_ = nullptr;
        }
    }
        break;
    default:
    {

    }
    }

    switch(mode)
    {
    case GPort_add:
    {
        QPixmap new_cursor_pix(":/res/icons/icons/vertex_add.svg");
        QCursor new_cursor(new_cursor_pix.scaled(ICON_SIZE,Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    case GPort_delete:
    {
        QPixmap new_cursor_pix(":/res/icons/icons/vertex_remove.svg");
        QCursor new_cursor(new_cursor_pix.scaled(ICON_SIZE,
                                                 Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    case GPort_startAddEdge:
    {
        QPixmap new_cursor_pix(":/res/icons/icons/edge_start.svg");
        QCursor new_cursor(new_cursor_pix.scaled(ICON_SIZE,
                                                 Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    case GPort_finAddEdge:
    {
        QPixmap new_cursor_pix(":/res/icons/icons/edge_finish.svg");
        QCursor new_cursor(new_cursor_pix.scaled(ICON_SIZE,
                                                 Qt::KeepAspectRatio));
        QApplication::setOverrideCursor(new_cursor);
    }
        break;
    default:
    {

    }
    }

    _mode_ = mode;

}
void GViewPort::setControlState(GPort_Controls mode, bool state)
{
    state?_controls_state_|=mode:_controls_state_&=~mode;
    return;
}


void GViewPort::setRadius(int radius)
{
    _vertex_radius_ = radius;
//    for(GViewItem* vertex: _vertices_)
//    {
//        vertex->setRadius(radius);
//    }
    for(int i = 0; i <_vertices_->size();i++)
    {
        _vertices_->at(i)->setRadius(radius);
    }
    for(GViewEdge* edge: _edges_)
    {
        edge->setVertRadius(radius);
    }
    return;
}

void GViewPort::mousePressEvent(QMouseEvent* m_event)
{
    GViewItem* item = grabGItem(m_event);
    if(!item)
    {
        if(_controls_state_&GPort_Ctr_SceneDrag)
        {
            _last_pos_ = m_event->pos();
            setControlState(GPort_Ctr_SceneDragMode,true);
            _saved_cursor_=cursor();
            setCursor(Qt::ClosedHandCursor);
            m_event->accept();
        }
        QGraphicsView::mousePressEvent(m_event);
        return;
    }
    switch(_mode_)
    {
    case GPort_add:
    case GPort_delete:
    {
        m_event->accept();
        //QGraphicsView::mousePressEvent(m_event);
        //return;
    }
        break;
    case GPort_finAddEdge:
    {
        finishAddEdge(item);
        selectItem(item);
    }
        break;
    case GPort_finDelEdge:
    {
        finishDelEdge(item);
        selectItem(item);
    }
        break;
    default:
    {
        selectItem(item);
    }
    }
    QGraphicsView::mousePressEvent(m_event);
    return;
}

void GViewPort::createItem(const QPoint& pos)
{
    GViewItem* item = new GViewItem(_vertex_radius_, &_no_image_,
                                    "Vertex N"+
                                    QString::number(_counter_++),
                                    Qt::gray
                                    );
    addItem(item,pos);
    selectItem(item);
    if(QApplication::overrideCursor())
    {
        QApplication::restoreOverrideCursor();
    }
    return;
}

void GViewPort::mouseReleaseEvent(QMouseEvent* m_event)
{
    switch(_mode_)
    {
    case GPort_add:
    {
        createItem(m_event->pos());
        m_event->accept();
    }
        break;
    case GPort_delete:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            deleteItem(g_item);
            QApplication::restoreOverrideCursor();
            m_event->accept();
        }
    }
        break;
    case GPort_startAddEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            startAddEdge(g_item);
        }
    }
        break;
    case GPort_startDelEdge:
    {
        if(GViewItem* g_item = grabGItem(m_event))
        {
            startDelEdge(g_item);
        }
    }
        break;
    case GPort_finAddEdge:
    case GPort_finDelEdge:
    {
        QGraphicsView::mouseReleaseEvent(m_event);
        return;
    }
        break;
    default:
    {
        GViewItem * item = grabGItem(m_event);
        selectItem(item);
    }
    }
    if(_controls_state_&GPort_Ctr_SceneDrag)
    {
        setControlState(GPort_Ctr_SceneDragMode,false);
        setCursor(_saved_cursor_);
        m_event->accept();
    }
    QGraphicsView::mouseReleaseEvent(m_event);
    return;
}

void GViewPort::mouseMoveEvent(QMouseEvent* m_event)
{
    switch(_mode_)
    {
    case GPort_finAddEdge:
    case GPort_finDelEdge:
    {
        if(_new_edge_)
        {
            _new_edge_->searchDestination(mapToScene(m_event->pos()));
        }
    }
        break;
    default:
    {
        if(_controls_state_&GPort_Ctr_SceneDragMode)
        {
            QPoint diff = m_event->pos()-_last_pos_;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value()-diff.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value()-diff.y());
            _last_pos_ =m_event->pos();
            m_event->accept();
        }
    }
    }
    QGraphicsView::mouseMoveEvent(m_event);
    return;
}

void GViewPort::contextMenuEvent(QContextMenuEvent* c_event)
{
    QMenu* menu = nullptr;
    QGraphicsItem* cap_item = scene()->itemAt(mapToScene(c_event->pos()),transform());
    GViewItem* g_item = qgraphicsitem_cast<GViewItem*>(cap_item);
    if(g_item)
    {
        menu = new QMenu("Дзеянні з вяршыняй:");
        QAction* act_pin = menu->addAction(QIcon(QPixmap(
                              (g_item->flags()&QGraphicsItem::ItemIsMovable)?
                                   ":/res/icons/icons/pin_free.svg":
                                   ":/res/icons/icons/pin_lock.svg").scaled(
                                                        ICON_SIZE,Qt::KeepAspectRatio)),
                              (g_item->flags()&QGraphicsItem::ItemIsMovable)?
                                               "Прычапіць":"Адчапіць");
        act_pin->setCheckable(true);
        act_pin->setChecked(!(g_item->flags()&QGraphicsItem::ItemIsMovable));

        QAction* act_del = menu->addAction(QIcon(QPixmap(
                             ":/res/icons/icons/vertex_remove.svg").
                                scaled(ICON_SIZE,Qt::KeepAspectRatio)),"Выдаліць");
        QAction* act_edge = menu->addAction(QIcon(QPixmap(
                             ":/res/icons/icons/edge_start.svg").
                                scaled(ICON_SIZE,Qt::KeepAspectRatio)),"Пачаць сувязь");
        QAction* act_select = menu->exec(c_event->globalPos());
        if(act_select== act_pin)
        {
            if(!act_pin->isChecked())
            {
                g_item->setFlag(QGraphicsItem::ItemIsMovable,true);
            }
            else
            {
                g_item->setFlag(QGraphicsItem::ItemIsMovable,false);
            }
            g_item->update();
        }
        else if(act_select ==act_del)
        {
            setMode(GPort_delete);
            deleteItem(g_item);
            QApplication::restoreOverrideCursor();
        }
        else if(act_select==act_edge)
        {
            setMode(GPort_startAddEdge);
            startAddEdge(g_item);
        }
        else
        {
            emit gviewMessage("Menu called an imposible action.");
        }

    }
    else if(!cap_item)
    {
        menu = new QMenu("Опцыі:");
        QAction* act_add = menu->addAction(
                    QIcon(QPixmap(":/res/icons/icons/vertex_add.svg").
                          scaled(ICON_SIZE,Qt::KeepAspectRatio)),"Дадаць");
        QAction* act_select = menu->exec(c_event->globalPos());
        if(act_select == act_add)
        {
            setMode(GPort_add);
            createItem(c_event->pos());
        }
        else
        {
            emit gviewMessage("Menu called an imposible action.");
        }
    }
    else
    {
        QGraphicsView::contextMenuEvent(c_event);
        return;
    }
    if(menu)
    {
        delete menu;
    }
    c_event->accept();
    return;
}

void GViewPort::selectItem(GViewItem* selected_item, bool outside)
{
    if(selected_item)
    {
        if(selected_item != _selected_vertex_)
        {
            if(_selected_vertex_)
                _selected_vertex_->setSelected(false);
            _selected_vertex_ = selected_item;
            _selected_vertex_->setSelected(true);
            if(outside)
            {
                return;
            }
            QString selected("Selected Item: ");
            selected.append(_selected_vertex_->info());
            emit gviewMessage(selected);
            emit viewNewSelect(_selected_vertex_);
        }
    }
///! На выпадак, калі трэба "здымаць вылучэнне" мэта працягу кода
//    else
//    {
//        if(_selected_vertex_)
//        {
//            _selected_vertex_->setSelected(false);
//            _selected_vertex_ = nullptr;
//        }
//    }
    return;
}

void GViewPort::outsideNewSelect(GViewItem* selected_item)
{
    selectItem(selected_item,true);
    return;
}


bool GViewPort::loadListGraph(const ListGraph& graph)
{
    std::vector<int> vert_ids(graph.getIDlist());
    QList<QPair<int,GViewItem*>> items;
    for(const int& id:vert_ids)
    {
#ifdef LIST_STR_VAL
        GViewItem* item = new GViewItem(_vertex_radius_,&_no_image_,
                                        QString(graph.value(id).c_str()),Qt::gray);
#else
        GViewItem* item = new GViewItem(_vertex_radius_,QString::number(graph.value(id)),Qt::gray);
#endif
        items.push_back(qMakePair(id,item));
    }
    QList<GViewEdge*> edges;
    auto iter = items.cbegin();
    while(iter!=items.cend())
    {
        std::vector<int> edge_ids = graph.getEdges(iter->first);
        if(edge_ids.size())
        {
            for(const int&edge: edge_ids)
            {
                auto edged_item = std::find_if(items.cbegin(),items.cend(),
                          [edge](const QPair<int,GViewItem*>& vert)
                    {return vert.first==edge;});
                if(edged_item==items.cend())
                {
                    std::for_each(items.begin(),
                                  items.end(),
                                  [](QPair<int,GViewItem*>& pair){delete pair.second;});
                    qDeleteAll(edges.begin(),edges.end());
                    return false;
                }
                bool is_exist= false;
                for(GViewEdge* exst_edge:edges)
                {
                    if(exst_edge->destination()==iter->second &&
                            exst_edge->source()==edged_item->second)
                    {
                        exst_edge->setDirected(false);
                       is_exist = true;
                       break;
                    }
                }
                if(is_exist)
                {
                    continue;
                }
                GViewEdge* new_edge =
                        new GViewEdge(iter->second,
                                      edged_item->second,
                                      _vertex_radius_,true);
                new_edge->setWeight(_vertex_radius_*10.0);
                iter->second->addEdge(new_edge);
                edged_item->second->addEdge(new_edge);
                edges.push_back(new_edge);
            }
        }
        ++iter;
    }
    clear();
    iter = items.cbegin();
    QPointF central_p = scene()->sceneRect().center();
    while(iter!=items.cend())
    {
        bool free_space = false;
        const qreal shift = _vertex_radius_*2.0;
        while(!free_space)
        {
            switch(QRandomGenerator::system()->bounded(0,4))
            {
            case 0:
            {
                central_p.setX(central_p.x()+shift);
            }
                break;
            case 1:
            {
                central_p.setY(central_p.y()+shift);
            }
                break;
            case 2:
            {
                central_p.setX(central_p.x()-shift);
            }
                break;
            case 3:
            {
                central_p.setY(central_p.y()-shift);
            }
                break;
            }
            if(!scene()->itemAt(central_p,transform()))
            {
                free_space = true;
            }
        }
        scene()->addItem(iter->second);
        iter->second->setPos(central_p);
        _vertices_->addItem(iter->second);
        ++iter;
    }
    for(GViewEdge* edge:edges)
    {
        scene()->addItem(edge);
        _edges_.push_back(edge);
        ///__________________
        edge->setWeight(_vertex_radius_*8.0);
        /// _________________
    }
    setForceCalc(true,false);
    emit internalForceArrangeAct();
    return true;
}


void GViewPort::itemMoved()
{
    if(!_timer_.isActive())
    {
        _timer_.start(1000/25,this);
    }
    return;
}

void GViewPort::timerEvent(QTimerEvent * t_event)
{
    Q_UNUSED(t_event)

    auto it = _vertices_->begin();
    while(it!= _vertices_->end())
    {
        (*it)->calcForce();
        ++it;
    }
    bool item_mov = false;
    it = _vertices_->begin();
    while(it!= _vertices_->end())
    {
        if((*it)->advPosition())
        {
            item_mov = true;
        }
        ++it;
    }
    if(!item_mov)
    {
        _timer_.stop();
    }
    return;
}

void GViewPort::setForceCalc(bool state, bool fixateEdgeLength)
{
    if(fixateEdgeLength)
    {
        for(GViewEdge* edge:_edges_)
        {
            edge->fixateLength();
        }
    }
    auto it = _vertices_->begin();
    while(it!=_vertices_->end())
    {
        (*it)->setForceCalc(state);
        ++it;
    }
    if(state)
        itemMoved();
    return;
}

void GViewPort::gatherInfo(nest_vert_map &vertices,
                           nest_vert_map &edges) const
{

    if(!vertices.empty())
    {
        qDeleteAll(vertices);
        vertices.clear();
    }
    if(!edges.empty())
    {
        qDeleteAll(edges);
        edges.clear();
    }
    _vertices_->gatherItemInfo(vertices);
    for(GViewEdge* edge:_edges_)
    {
        vert_map* vertex_m= new vert_map;
        edge->gatherInfo(vertex_m);
        edges.append(vertex_m);
    }
    return;
}

void GViewPort::loadInfo(const nest_vert_map& vertices,
              const nest_vert_map& edges)
{
    clear();
    QMap<QString,GViewItem*> obj_map;
    for(const vert_map*vertex:vertices)
    {
        qreal x = 0;
        qreal y = 0;
        int radius = 0;
        QRgb colour;
        QString data;
        QString code;
        vert_map::const_iterator it = vertex->find("x");
        if(it==vertex->constEnd())
        {
            continue;
        }
        x = it.value().toDouble();
        it = vertex->find("y");
        if(it==vertex->constEnd())
        {
            continue;
        }
        y = it.value().toDouble();
        it = vertex->find("radius");
        if(it==vertex->constEnd())
        {
            continue;
        }
        radius = it.value().toInt();
        it = vertex->find("color");
        if(it==vertex->constEnd())
        {
            continue;
        }
        colour = it.value().toInt();
        it = vertex->find("ptr_id");
        if(it==vertex->constEnd())
        {
            continue;
        }
        code = it.value();
        it = vertex->find("data");
        if(it==vertex->constEnd())
        {
            continue;
        }
        data = it.value();

        GViewItem* item = new GViewItem(radius, &_no_image_,
                                        data,
                                        colour);
        setMode(GPort_add);
        addItem(item,mapFromScene(x,y));
        obj_map.insert(code,item);
    }
    for(const vert_map*edge:edges)
    {
        GViewItem* source = nullptr;
        GViewItem* dest = nullptr;
        bool directed = false;
        vert_map::const_iterator it = edge->find("source");
        if(it==edge->constEnd())
        {
            continue;
        }
        if(!obj_map.contains(it.value()))
        {
            continue;
        }
        source = obj_map.value(it.value());
        it = edge->find("destination");
        if(it==edge->constEnd())
        {
            continue;
        }
        if(!obj_map.contains(it.value()))
        {
            continue;
        }
        dest = obj_map.value(it.value());
        it = edge->find("directed");
        if(it==edge->constEnd())
        {
            continue;
        }
        directed = it.value().toInt();
        addEdge(source,dest,directed);
    }
    return;
}

void GViewPort::wheelEvent(QWheelEvent* w_event)
{
    if(!(_controls_state_&GPort_Ctr_Zoom)/*_zoom_mode_*/)
    {
        w_event->ignore();
        return QGraphicsView::wheelEvent(w_event);
    }

    if(w_event->angleDelta().y()>0)
    {
        zoomIn();
    }
    else
    {
        zoomOut();
    }
    return;
}

void GViewPort::zoomIn()
{
    qreal cur_scale = transform().m11();
    if(cur_scale<3.0)
    {
        scale(1.25,1.25);
    }
    return;
}

void GViewPort::zoomOut()
{
    qreal cur_scale = transform().m11();
    if(cur_scale>0.2)
    {
        scale(0.8,0.8);
    }
    return;
}

void GViewPort::setZoomMode(bool state)
{
    if(state)
    {
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    }
    else
    {
        setTransformationAnchor(QGraphicsView::NoAnchor);
    }
    return;
}
