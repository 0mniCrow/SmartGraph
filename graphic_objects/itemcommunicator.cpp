#include "itemcommunicator.h"
#include "abstractGrItem.h"

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
    if(_main_port_)
    {
        //_main_port_->time_changed()
    }
    return;

}

unsigned long long ItemCommunicator::getCurTime() const noexcept
{
    return _cur_time_;
}

void ItemCommunicator::startToolTipTimer(AbstractGrItem* gr_sender, const QPoint& pos)
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

void ItemCommunicator::callEditWindow(AbstractGrItem* gr_sender, const QPoint& pos)
{
    if(!_edit_window_)
    {
        QString data;
        //!TODO - get data from Item and save it in variable 'data'
        _edit_window_ = new GViewEdit(data);
        //connect(_edit_window_,&GViewEdit::valueChanged,this,&GViewItem::getNewInfo);
        //connect(this,&GViewItem::changedExternally,_edit_window_,&GViewEdit::updateFields);
    }
    if(!pos.isNull())
    {
        _edit_window_->move(pos);
    }
    else if(!_tip_pos_.isNull())
    {
        _edit_window_->move(_tip_pos_);
    }
    _edit_window_->show();
    return;
}

void ItemCommunicator::callToolTipWindow(AbstractGrItem* gr_sender, const QPoint& pos)
{
    if(!_tooltip_window_)
    {
        QString gr_data;
        //!TODO fill gr_data with abstract Item data
        _tooltip_window_ = new GViewToolTip(gr_data);
        //connect(this,&GViewItem::changedExternally,_tooltip_window_,&GViewToolTip::updateFields);
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

void ItemCommunicator::timeOut()
{
    callToolTipWindow(_cur_working_item_,_tip_pos_);
    return;
}
