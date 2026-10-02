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
    _data_group_(nullptr),_data_layout_(nullptr),_current_item_id_(0)
{
    setWindowFlag(Qt::Popup,true);
    resize(200,100);
    generateMainLayout();
    return;
}

void GViewToolTip::generateMainLayout()
{
    QLayout* main_layout = layout();
    if(main_layout)
    {
        if(_data_group_)
        {
            main_layout->removeWidget(_data_group_);
        }
        delete main_layout;
    }
    main_layout = new QVBoxLayout();
    if(_data_group_)
    {
        main_layout->addWidget(_data_group_);
    }
    setLayout(main_layout);
    return;
}

bool GViewToolTip::updateGrLayout()
{
    if(isVisible())
    {
        close();
    }
    if(_data_group_)
    {
        if(_data_layout_)
        {
            delete _data_layout_;
            _data_layout_ = nullptr;
        }
        delete _data_group_;
        _data_group_ = nullptr;
    }
    generateDataGroup(_current_item_type_);
    if(!_data_layout_)
    {
        return false;
    }
    _data_group_ = new QGroupBox();
    _data_group_->setLayout(_data_layout_);
    layout()->addWidget(_data_group_);
    return true;
}

bool GViewToolTip::setItemType(QStringView item_type)
{
    if(item_type==_current_item_type_)
    {
        return false;
    }
    return updateGrLayout();
}

bool GViewToolTip::setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data)
{
    if(item_type!=_current_item_type_)
    {
        if(!setItemType(item_type))
        {
            return false;
        }
    }
    _current_item_id_ = id;
    loadDataList(data);
    return true;
}

void GViewToolTip::loadDataList(const QList<QPair<QString,QVariant>>& data)
{
    _current_data_.clear();
    _current_data_ = data;
    _current_data_.detach();
    return;
}

const QList<QPair<QString,QVariant>>& GViewToolTip::getDataList() const
{
    return _current_data_;
}

QStringView GViewToolTip::getCurrentItemType()
{
    return _current_item_type_;
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
    if(_data_layout_ && item_type == _current_item_type_)
    {
        return;
    }

    if(!item_type.compare(QString("StaticGrItem")))
    {
        if(_data_layout_)
        {
            delete _data_layout_;
        }
        _data_layout_ = new QVBoxLayout();
        QTextEdit* text_box = new QTextEdit();
        QString text_box_name = "QTextEdit_text_data";
        text_box->setReadOnly(true);
        text_box->setObjectName(text_box_name);
        _data_layout_->addWidget(text_box);
        QList<QPair<QString,QVariant>> data_list{std::make_pair(text_box_name,QVariant())};
        loadDataList(data_list);
    }
    return;
}

void GViewToolTip::updateValues()
{
    if(_current_item_type_=="StaticGrItem")
    {
        QTextEdit * text_box = _data_layout_->findChild<QTextEdit*>(_current_data_.first().first);
        if(text_box)
        {
            text_box->setText(_current_data_.first().second.toString());
        }
    }
    return;
}
