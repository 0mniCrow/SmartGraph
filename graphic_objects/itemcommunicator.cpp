#include "itemcommunicator.h"
#include "abstractgrqtitem.h"
#include "graphic_objects/staticgritem.h"
#include "gviewport.h"

ItemCommunicator::ItemCommunicator(GViewPort *port, QObject *parent)
    : QObject{parent},
      _main_port_(port),
      _cur_time_(0),
      _tooltip_window_(nullptr),
      _edit_window_(nullptr),
      _pic_load_dialog_(nullptr),
      _cur_working_item_(nullptr)
{
    _tip_timer_.setSingleShot(true);
    connect(&_tip_timer_,&QTimer::timeout,this,&ItemCommunicator::timeOut);
    return;
}

void ItemCommunicator::setCurTime(unsigned long long new_time)
{
    _cur_time_ = new_time;
    return;

}

unsigned long long ItemCommunicator::getCurTime() const noexcept
{
    return _cur_time_;
}

void ItemCommunicator::startToolTipTimer(AbstractGrQtItem* gr_sender, const QPoint& pos)
{
    if(_tooltip_window_ && _tooltip_window_->isVisible())
    {
        _tooltip_window_->close();
    }
    if(_tip_timer_.isActive())
    {
        stopToolTipTimer();
    }
    _tip_pos_ = pos;
    _cur_working_item_ = gr_sender;
    _tip_timer_.start(TIMER_DELAY);
    return;
}

void ItemCommunicator::stopToolTipTimer()
{
    _tip_timer_.stop();
    return;
}

void ItemCommunicator::setDefImage(const QString& imgAddr)
{
    QPixmap def_pic(imgAddr);
    if(!def_pic.isNull())
    {
        _def_image_ = def_pic;
    }
    return;
}

const QPixmap& ItemCommunicator::getDefImage() const
{
    return _def_image_;
}

void ItemCommunicator::parseItemData(AbstractGrQtItem* item, QList<QPair<QString,QVariant>>& container)
{
    container.clear();
    if(!_edit_window_)
    {
        return;
    }
    QStringView itemType = item->getObjectName();
    if(itemType == QString("StaticGrItem"))
    {
        StaticGrItem* static_item = dynamic_cast<StaticGrItem*>(item);
        _edit_window_->setItemType(itemType);
        container = _edit_window_->getDataList();
        container.first().second = QVariant(static_item->getGrData());
    }
    return;
}

void ItemCommunicator::callEditWindow(AbstractGrQtItem* gr_sender, const QPoint& pos)
{
    if(!_edit_window_)
    {
        _edit_window_ = new GViewEdit;
        connect(_edit_window_,&GViewEdit::itemValueChanged,this,&ItemCommunicator::editWindowUpdated);

    }
    if(!gr_sender->getGrID())
    {
        return;
    }
    QList<QPair<QString,QVariant>> data_container;
    parseItemData(gr_sender,data_container);
    _edit_window_->setDataList(gr_sender->getGrID(),gr_sender->getObjectName(),data_container);
    if(data_container.isEmpty())
    {
        return;
    }

    if(!pos.isNull())
    {
        _edit_window_->move(pos);
    }
    else if(!_tip_pos_.isNull())
    {
        _edit_window_->move(_tip_pos_);
    }
    if(_tip_timer_.isActive())
    {
        stopToolTipTimer();
    }
    if(_tooltip_window_ && _tooltip_window_->isVisible())
    {
        _tooltip_window_->close();
    }
    _cur_working_item_ = gr_sender;
    _edit_window_->show();
    return;
}

void ItemCommunicator::callToolTipWindow(AbstractGrQtItem* gr_sender, const QPoint& pos)
{
    if(!_tooltip_window_)
    {
        _tooltip_window_= new GViewToolTip();
    }
    QList<QPair<QString,QVariant>> data_container;
    parseItemData(gr_sender,data_container);
    _tooltip_window_->setDataList(gr_sender->getGrID(),gr_sender->getObjectName(),data_container);
    if(data_container.isEmpty())
    {
        return;
    }
    if(!pos.isNull())
    {
        _tooltip_window_->move(pos);
    }
    else if(!_tip_pos_.isNull())
    {
        _tooltip_window_->move(_tip_pos_);
    }
    _tooltip_window_->show();
    return;
}

void ItemCommunicator::itemIsMoved()
{
    if(_main_port_)
    {
        _main_port_->itemMoved();
    }
    return;
}

void ItemCommunicator::setArrowSize(qreal arrow_size)
{
    _arrow_size_ = arrow_size>DEFAULT_ARROW_SIZE?arrow_size:DEFAULT_ARROW_SIZE;
    if(_main_port_)
    {
        _main_port_->arrowSizeChanged();
    }
    return;
}

qreal ItemCommunicator::getArrowSize() const noexcept
{
    return _arrow_size_;
}

void ItemCommunicator::timeOut()
{
    callToolTipWindow(_cur_working_item_,_tip_pos_);
    return;
}

void ItemCommunicator::parseEditWindowData(AbstractGrQtItem* item)
{
    if(!_edit_window_)
    {
        return;
    }
    QStringView itemType = item->getObjectName();
    if(itemType!=_edit_window_->getCurrentItemType())
    {
        return;
    }
    if(itemType == QString("StaticGrItem"))
    {
        StaticGrItem* static_item = dynamic_cast<StaticGrItem*>(item);
        const QList<QPair<QString,QVariant>>&data(_edit_window_->getDataList());
        static_item->setGrData(data.first().second.toString(),ItemDataInterface<QString>::DC_External);
    }
    return;
}

void ItemCommunicator::editWindowUpdated(uint item_id)
{
    if(!_edit_window_)
    {
        return;
    }

    AbstractGrQtItem* item = _main_port_->getGrItemByID(item_id);
    parseEditWindowData(item);
    emit grItemUpdaded(item_id);
    return;
}

QGraphicsView * ItemCommunicator::getMainPort() const noexcept
{
    return _main_port_;
}
