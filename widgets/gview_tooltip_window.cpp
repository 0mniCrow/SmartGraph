#include "gview_tooltip_window.h"

GViewToolTip::GViewToolTip(const QString& data, QWidget* tata):QWidget(tata)
{
    _info_ = new QLabel;
    _info_->setText(data);
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_info_);
    setLayout(layout);
    setStyleSheet(
                                  "QLabel {color : darkRed; "
                                  "background-color:yellow} "
                                  );
    setWindowFlag(Qt::Popup,true);
    resize(200,100);
    return;
}

GViewToolTip::GViewToolTip(QWidget* tata):QWidget(tata)
{
    setWindowFlag(Qt::Popup,true);
    resize(200,100);
}

void GViewToolTip::updateGrLayout()
{
    if(isVisible())
    {
        close();
    }
    QLayout* old_layout = layout();
    if(old_layout)
    {
        if(_widget_layer_)
        {
            layout()->removeWidget(_widget_layer_);
            delete _widget_layer_;
        }
        delete old_layout;
    }
    QVBoxLayout* new_layout = new QVBoxLayout();
    _widget_layer_ = new QGroupBox();
    new_layout->addWidget(_widget_layer_);
    setLayout(new_layout);
}
void GViewToolTip::setItemType(QStringView item_type)
{
    if(item_type==_current_item_type_)
    {
        return;
    }
    generateWidgetLayer(item_type);
    updateGrLayout();
    return;
}
bool GViewToolTip::setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data)
{

}
const QList<QPair<QString,QVariant>>& GViewToolTip::getDataList() const
{

}
QStringView GViewToolTip::getCurrentItemType()
{

}
uint GViewToolTip::getCurrentItemID()const noexcept
{
    return _current_item_id_;
}

void GViewToolTip::updateFields(const QString& new_val)
{
    _info_->setText(new_val);
    return;
}

void GViewToolTip::generateWidgetLayer(QStringView item_type)
{

}

void GViewToolTip::updateValues()
{

}
