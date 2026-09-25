#include "gview_edit_window.h"
#include "graphic_objects/staticgritem.h"
GViewEdit::GViewEdit(const QString& data, QWidget* tata):QWidget(tata)
{
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint,true);
    _save_button_ = new QPushButton("Save 'n Close");
    _close_button_ = new QPushButton("Close");
    _apply_button_ = new QPushButton("Apply");
    connect(_save_button_,&QPushButton::clicked,this,&GViewEdit::manualSave);
    connect(_close_button_,&QPushButton::clicked,this,&GViewEdit::manualClose);
    connect(_apply_button_,&QPushButton::clicked,this,&GViewEdit::manualApply);
    QHBoxLayout * button_line = new QHBoxLayout;
    button_line->addWidget(_save_button_);
    button_line->addWidget(_close_button_);
    button_line->addWidget(_apply_button_);

    _text_ = new QTextEdit;
    _text_->setText(data);
    _original_text_=data;
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_text_);
    layout->addLayout(button_line);
    setLayout(layout);
    setStyleSheet(
                                  "QTextEdit {color : darkRed; "
                                  "background-color:yellow} "
                                  "QPushButton {"
                                  "background-color: red;"
                                  "border-style: outset;"
                                  "border-width: 2px;"
                                  "border-radius: 10px;"
                                  "border-color: beige;"
                                  "font: bold 14px;"
                                  "min-width: 10em;"
                                  "padding: 6px;"
                                  "}"
                                  "QPushButton:pressed {"
                                  "background-color: rgb(224, 12, 77);"
                                  "border-style: inset;}");
    resize(300,150);
    return;
}

GViewEdit::GViewEdit(AbstractGrItem* first_item, QWidget* tata):QWidget(tata)
{
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint,true);
    _save_button_ = new QPushButton("Save 'n Close");
    _close_button_ = new QPushButton("Close");
    _apply_button_ = new QPushButton("Apply");
    connect(_save_button_,&QPushButton::clicked,this,&GViewEdit::manualSave);
    connect(_close_button_,&QPushButton::clicked,this,&GViewEdit::manualClose);
    connect(_apply_button_,&QPushButton::clicked,this,&GViewEdit::manualApply);
    QHBoxLayout * button_line = new QHBoxLayout;
    button_line->addWidget(_save_button_);
    button_line->addWidget(_close_button_);
    button_line->addWidget(_apply_button_);
    _text_ = new QTextEdit;
    QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(_text_);
    layout->addLayout(button_line);
    setLayout(layout);
    resize(300,150);
    if(!first_item)
    {
        return;
    }
    QStringView item_type = first_item->getObjectName();
    if(item_type == QString("StaticGrItem"))
    {
        StaticGrItem* item = dynamic_cast<StaticGrItem*>(first_item);
        if(!item)
        {
            return;
        }
        _original_text_ = item->getGrData();
        _text_->setText(_original_text_);

    }
    return;
}



GViewEdit::GViewEdit(QWidget* tata):QWidget(tata),
    _widget_layer_(nullptr),_current_item_id_(0),_save_button_(nullptr),
    _close_button_(nullptr),_apply_button_(nullptr),_main_layout_(nullptr),_interface_layout_(nullptr)
{
    setWindowModality(Qt::WindowModality::ApplicationModal);
    setWindowFlag(Qt::FramelessWindowHint,true);
    generateMainInterface();
    _main_layout_ = new QVBoxLayout();
    setLayout(_main_layout_);
    resize(300,150);
}

void GViewEdit::generateMainInterface()
{
    if(_save_button_)
    {
        disconnect(_save_button_,&QPushButton::clicked,this,&GViewEdit::manualSave);
        delete _save_button_;
    }
    if(_close_button_)
    {
        disconnect(_close_button_,&QPushButton::clicked,this,&GViewEdit::manualClose);
        delete _close_button_;
    }
    if(_apply_button_)
    {
        disconnect(_apply_button_,&QPushButton::clicked,this,&GViewEdit::manualApply);
        delete _apply_button_;
    }
    if(_interface_layout_)
    {
        delete _interface_layout_;
    }
    _save_button_ = new QPushButton("Save 'n Close");
    _close_button_ = new QPushButton("Close");
    _apply_button_ = new QPushButton("Apply");
    connect(_save_button_,&QPushButton::clicked,this,&GViewEdit::manualSave);
    connect(_close_button_,&QPushButton::clicked,this,&GViewEdit::manualClose);
    connect(_apply_button_,&QPushButton::clicked,this,&GViewEdit::manualApply);
    _interface_layout_ = new QHBoxLayout;
    _interface_layout_->addWidget(_save_button_);
    _interface_layout_->addWidget(_close_button_);
    _interface_layout_->addWidget(_apply_button_);
    return;
}

void GViewEdit::updateLayout()
{
    if(isVisible())
    {
        close();
    }
    if(_main_layout_&&_interface_layout_)
    {
        _main_layout_->removeItem(_interface_layout_);
        delete _main_layout_;
    }
    _main_layout_ = new QVBoxLayout();
    if(_widget_layer_)
    {
        _main_layout_->addWidget(_widget_layer_);
    }
    if(_interface_layout_)
    {
    _main_layout_->addItem(_interface_layout_);
    }
    setLayout(_main_layout_);
}

void GViewEdit::generateWidgetLayer(QStringView item_type)
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
        //Каб карыстальнік разумеў, якія палі й у якой паслядоўнасьці будуць адлюстраваны
        _original_data_.clear();
        _original_data_.append(std::make_pair(text_box_name,QVariant()));
    }
    return;
}

bool GViewEdit::loadDataList(const QList<QPair<QString,QVariant>>& data)
{
    for(int i = 0; i<_original_data_.size();++i)
    {
        if(data.at(i).first!=_original_data_.at(i).first)
        {
            return false;
        }
        _original_data_[i].second=data.at(i).second;
    }
    updateValues();
    return true;
}

void GViewEdit::updateValues()
{
    if(_current_item_type_=="StaticGrItem")
    {
        QTextEdit * text_box = _widget_layer_->findChild<QTextEdit*>(_original_data_.first().first);
        if(text_box)
        {
            text_box->setText(_original_data_.first().second.toString());
        }
    }
    return;
}

void GViewEdit::saveValues()
{
    if(_current_item_type_=="StaticGrItem")
    {
        QTextEdit * text_box = _widget_layer_->findChild<QTextEdit*>(_original_data_.first().first);
        if(text_box)
        {
            _original_data_.first().second.setValue(text_box->toPlainText());
        }
    }
    return;
}

bool GViewEdit::checkDataForChanges()
{
    if(_current_item_type_=="StaticGrItem")
    {
        QTextEdit * text_box = _widget_layer_->findChild<QTextEdit*>(_original_data_.first().first);
        if(text_box)
        {
            if(text_box->toPlainText()!=_original_data_.first().second.toString())
            {
                return false;
            }
        }
        return true;
    }
    return true;
}

void GViewEdit::setItemType(QStringView item_type)
{
    if(item_type==_current_item_type_)
    {
        return;
    }
    generateWidgetLayer(item_type);
    updateLayout();
    return;
}
QStringView GViewEdit::getCurrentItemType()
{
    return _current_item_type_;
}

bool GViewEdit::setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data)
{
    if(item_type!=_current_item_type_)
    {
        setItemType(item_type);
    }
    _current_item_id_ = id;
    return loadDataList(data);
}

uint GViewEdit::getCurrentItemID()const noexcept
{
    return _current_item_id_;
}

const QList<QPair<QString,QVariant>>& GViewEdit::getDataList() const
{
    return _original_data_;
}

void GViewEdit::manualSave()
{
    //!Obsolete
    if(_original_text_!=_text_->toPlainText())
    {
        manualApply();
    }
    //!new
    /*
    if(!checkDataForChanges())
    {
        saveValues();
        emit itemValueChanged(_current_item_id_);
    }
    */
    close();
    return;
}
void GViewEdit::manualClose()
{
    //!Obsolete
    if(_original_text_!=_text_->toPlainText())
    {
        QMessageBox mbx(this);
        mbx.setText("Зьмяненьні не былі захаваныя.");
        mbx.setInformativeText("Ці жадаеце Вы захаваць зьмяненьні?");
        mbx.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        mbx.setDefaultButton(QMessageBox::Save);
        int ret = mbx.exec();
        switch(ret)
        {
        case QMessageBox::Save:
        {
            manualApply();
        }
            break;
        case QMessageBox::Discard:
        {
            _text_->setText(_original_text_);
        }
            break;
        case QMessageBox::Cancel:
        {
            return;
        }
            break;
        }
    }
    //!New
    /*
    if(!checkDataForChanges())
    {
        QMessageBox mbx(this);
        mbx.setText("Зьмяненьні не былі захаваныя.");
        mbx.setInformativeText("Ці жадаеце Вы захаваць зьмяненьні?");
        mbx.setStandardButtons(QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        mbx.setDefaultButton(QMessageBox::Save);
        int ret = mbx.exec();
        switch(ret)
        {
        case QMessageBox::Save:
        {
            manualApply();
        }
            break;
        case QMessageBox::Discard:
        {
            updateValues();
        }
            break;
        case QMessageBox::Cancel:
        {
            return;
        }
            break;
        }
    }
    */
    close();
    return;
}
void GViewEdit::manualApply()
{
    //!Obsolete
    if(_original_text_==_text_->toPlainText())
    {
        return;
    }
    _original_text_ = _text_->toPlainText();
    emit valueChanged(_original_text_);
    //!New:
    /*
    if(checkDataForChanges())
    {
        return;
    }
    saveValues();
    emit itemValueChanged(_current_item_id_);
    */
    return;
}

void GViewEdit::updateFields(const QString& new_val)
{
    _original_text_ = new_val;
    _text_->setText(_original_text_);
    return;
}

void GViewEdit::setData(const QMap<QString,QString>& fields)
{
    Q_UNUSED(fields)
    //!TODO - for further design of fields.
}

void GViewEdit::setData(const QString& data)
{
    _original_text_ = data;
    _text_->setText(data);
    return;
}

QString GViewEdit::getData() const
{
    return _original_text_;
}
