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

GViewToolTip::GViewToolTip(QWidget* tata):QWidget(tata),
    _widget_layer_(nullptr),_current_item_id_(0)
{
    QVBoxLayout * layout = new QVBoxLayout;
    setLayout(layout);
    setWindowFlag(Qt::Popup,true);
    resize(200,100);
    return;
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
        }
        delete old_layout;
    }
    QVBoxLayout* new_layout = new QVBoxLayout();
    generateDataGroup(_current_item_type_);
    if(_widget_layer_)
    {
        new_layout->addWidget(_widget_layer_);
    }
    setLayout(new_layout);
    return;
}

void GViewToolTip::setItemType(QStringView item_type)
{
    if(item_type==_current_item_type_)
    {
        return;
    }
    generateDataGroup(item_type);
    updateGrLayout();
    return;
}

bool GViewToolTip::setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data)
{
    if(item_type!=_current_item_type_)
    {
        setItemType(item_type);
    }
    _current_item_id_ = id;
    return loadDataList(data);
}

void GViewToolTip::setDataGroup(QGroupBox* widget_group)
{

}

bool GViewToolTip::loadDataList(const QList<QPair<QString,QVariant>>& data)
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

void GViewToolTip::generateDataGroup(QStringView item_type)
{
    if(!item_type.compare(QString("StaticGrItem")))
    {
        if(_widget_layer_)
        {
            delete _widget_layer_;
        }
        _widget_layer_ = new QGroupBox();
        QHBoxLayout* data_layout = new QHBoxLayout();
        QTextEdit* text_box = new QTextEdit();
        QString text_box_name = "QTextEdit_text_data";
        text_box->setObjectName(text_box_name);
        data_layout->addWidget(text_box);
        _widget_layer_->setLayout(data_layout);
    }
    return;
}

void GViewToolTip::updateValues()
{
    if(_current_item_type_=="StaticGrItem")
    {
        QTextEdit * text_box = _widget_layer_->findChild<QTextEdit*>(_current_data_.first().first);
        if(text_box)
        {
            text_box->setText(_current_data_.first().second.toString());
        }
    }
    return;
}
