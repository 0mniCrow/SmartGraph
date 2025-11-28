#include "infowidget.h"

InfoWidget::InfoWidget(bool read_only, QWidget *parent)
    : QWidget{parent},_hiding_timer_(this),_control_panel_(nullptr),
      _save_opt_(nullptr),_close_opt_(nullptr),_flags_(IW_NoFlags)
{
    //Таймер
    _hiding_timer_.setSingleShot(true);
    connect(&_hiding_timer_,&QTimer::timeout,this,&InfoWidget::closeLocal);

    //Слой для элементаў кіравання
    QLayout* cur_layout = layout();
    if(cur_layout)
    {
        delete cur_layout;
    }
    QVBoxLayout* new_layout = new QVBoxLayout();
    setLayout(new_layout);

    //Панэль кіравання для рэдактыруемых палёў
    QHBoxLayout* h_layout = new QHBoxLayout;
    _save_opt_ = new QPushButton(QIcon::fromTheme("save"),"Зах.");
    _close_opt_ = new QPushButton(QIcon::fromTheme("close"),"Закр.");
    h_layout->addWidget(_save_opt_);
    h_layout->addWidget(_close_opt_);
    connect(_save_opt_,&QPushButton::clicked,this,&InfoWidget::saveLocal);
    connect(_close_opt_,&QPushButton::clicked,this,&InfoWidget::closeLocal);
    _control_panel_ = new QGroupBox();
    _control_panel_->setLayout(h_layout);
    layout()->addWidget(_control_panel_);
    if(read_only)
    {
        _flags_|=IW_ReadOnly;
        _control_panel_->setVisible(false);
    }
    return;
}

InfoWidget::~InfoWidget()
{
    disconnect(&_hiding_timer_,&QTimer::timeout,this,&InfoWidget::closeLocal);
    disconnect(_save_opt_,&QPushButton::clicked,this,&InfoWidget::saveLocal);
    disconnect(_close_opt_,&QPushButton::clicked,this,&InfoWidget::closeLocal);
}

void InfoWidget::closeEvent(QCloseEvent*  cl_event)
{
    if((_flags_&IW_ReadOnly)||
            (_flags_&IW_OnTimer)||
            !hasChanged())
    {
        _flags_&=~IW_OnTimer;
        cl_event->accept();
        return;
    }
    QMessageBox mbx;
    mbx.setText("Не ўсе зробленыя Вамі зьмяненьні захаваны.");
    mbx.setInformativeText("Ці жадаеце спачатку захаваць зьмяненьні?");
    mbx.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    mbx.setDefaultButton(QMessageBox::Yes);
    mbx.setIcon(QMessageBox::Warning);
    int vynik =mbx.exec();
    switch(vynik)
    {
    case QMessageBox::Yes:
        emit saveRequest();
        [[fallthrough]];
    case QMessageBox::No:
    {
        cl_event->accept();
    }
        break;
    }
    emit closeRequest();
    return;
}

void InfoWidget::enterEvent(QEnterEvent* ent_event)
{
    if(_flags_&(IW_ReadOnly | IW_OnTimer))
    {
        _hiding_timer_.stop();
        _flags_&=~IW_OnTimer;
    }
    QWidget::enterEvent(ent_event);
    return;
}

void InfoWidget::leaveEvent(QEvent* lef_event)
{
    if(_flags_&IW_ReadOnly)
    {
        startClosingTimer();
    }
    QWidget::leaveEvent(lef_event);
    return;
}

void InfoWidget::mouseDoubleClickEvent(QMouseEvent* m_event)
{
    if(_flags_&IW_ReadOnly)
    {
        setReadOnly(false);
    }
    QWidget::mouseDoubleClickEvent(m_event);
    return;
}

bool InfoWidget::isContainerClass(const QString& class_name)
{
    return class_name == "QGroupBox";
}

bool InfoWidget::isEditableClass(const QString& class_name)
{
    return class_name == "QSpinBox" ||
            class_name == "QTextEdit" ||
            class_name == "QLineEdit" ||
            class_name == "QComboBox";
}

bool InfoWidget::isAllowedClass(const QString& class_name)
{
    return InfoWidget::isContainerClass(class_name) ||
            InfoWidget::isEditableClass(class_name)||
            class_name == "QLabel";
}

void InfoWidget::connectElement(const QString& type, QWidget* element)
{
    if(type == "QSpinBox")
    {
        QSpinBox* obj = qobject_cast<QSpinBox*>(element);
        connect(obj,&QSpinBox::valueChanged,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QTextEdit")
    {
        QTextEdit* obj = qobject_cast<QTextEdit*>(element);
        connect(obj,&QTextEdit::textChanged,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QLineEdit")
    {
        QLineEdit* obj = qobject_cast<QLineEdit*>(element);
        connect(obj,&QLineEdit::textEdited,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QComboBox")
    {
        QComboBox* obj = qobject_cast<QComboBox*>(element);
        connect(obj,&QComboBox::currentIndexChanged,this,&InfoWidget::catchElementSignal);
    }
    return;
}

void InfoWidget::disconnectElement(const QString& type, QWidget* element)
{
    if(type == "QSpinBox")
    {
        QSpinBox* obj = qobject_cast<QSpinBox*>(element);
        disconnect(obj,&QSpinBox::valueChanged,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QTextEdit")
    {
        QTextEdit* obj = qobject_cast<QTextEdit*>(element);
        disconnect(obj,&QTextEdit::textChanged,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QLineEdit")
    {
        QLineEdit* obj = qobject_cast<QLineEdit*>(element);
        disconnect(obj,&QLineEdit::textEdited,this,&InfoWidget::catchElementSignal);
    }
    else if(type == "QComboBox")
    {
        QComboBox* obj = qobject_cast<QComboBox*>(element);
        disconnect(obj,&QComboBox::currentIndexChanged,this,&InfoWidget::catchElementSignal);
    }
    return;
}

void InfoWidget::lockElement(const QString &type, QWidget* element, bool lock_status)
{
    if(!isEditableClass(type))
        return;
    element->setEnabled(lock_status);
}

bool InfoWidget::hasChanged() const
{
    QMap<QString,ObjReinforced>::const_iterator it = _elements_.constBegin();
    while(it!= _elements_.constEnd())
    {
        if(!isEditableClass(it.value()._widget_->metaObject()->className()))
        {
            ++it;
            continue;
        }
        QVariant val = getValue(it.value()._widget_);
        if(val!=it.value()._orig_value_)
        {
            return true;
        }
        ++it;
    }
    return false;
}

bool InfoWidget::loadValue(const QString& type, QWidget* element, const QVariant& value)
{
    if(type == "QSpinBox")
    {
        QSpinBox* obj = qobject_cast<QSpinBox*>(element);
        if(value.canConvert<int>())
        {
            obj->setValue(value.toInt());
        }
    }
    else if(type == "QTextEdit")
    {
        QTextEdit* obj = qobject_cast<QTextEdit*>(element);
        if(value.canConvert<QString>())
        {
            obj->setText(value.toString());
        }
    }
    else if(type == "QLineEdit")
    {
        QLineEdit* obj = qobject_cast<QLineEdit*>(element);
        if(value.canConvert<QString>())
        {
            obj->setText(value.toString());
        }
    }
    else if(type == "QComboBox")
    {
        QComboBox* obj = qobject_cast<QComboBox*>(element);
        if(value.canConvert<int>())
        {
            obj->setCurrentIndex(value.toInt());
        }
    }
    else
    {
        return false;
    }
    return true;
}

QVariant InfoWidget::getValue(QWidget* widget) const
{
    QString type(widget->metaObject()->className());
    QVariant answer;
    if(type == "QSpinBox")
    {
        QSpinBox* element = qobject_cast<QSpinBox*>(widget);
        if(element)
        {
            answer = QVariant::fromValue(element->value());
        }
    }
    else if(type == "QLineEdit")
    {
        QLineEdit* element = qobject_cast<QLineEdit*>(widget);
        if(element)
        {
            answer = QVariant::fromValue(element->text());
        }
    }
    else if(type == "QTextEdit")
    {
        QTextEdit* element = qobject_cast<QTextEdit*>(widget);
        if(element)
        {
            answer = QVariant::fromValue(element->toHtml());
        }
    }
    else if(type == "QComboBox")
    {
        QComboBox* element = qobject_cast<QComboBox*>(widget);
        if(element)
        {
            answer = QVariant::fromValue(element->currentText());
        }
    }
    else if(type == "QGroupBox")
    {
        const QObjectList& elem_list = widget->children();
        QString str_answer;
        for(QObject* child:elem_list)
        {
            QWidget* child_wgt = qobject_cast<QWidget*>(child);
            if(child_wgt)
            {
                str_answer.append(getValue(child_wgt).toString()+" ");
            }
        }
        answer = QVariant::fromValue(str_answer);
    }
    return answer;
}

void InfoWidget::addElement(QWidget* element)
{
    QString object_class(element->metaObject()->className());

    if(isAllowedClass(object_class))
    {
        _elements_.insert(element->objectName(),ObjReinforced(element,getValue(element)));
        //Перасоўванне панэлі кіравання на апошні шэраг на віджэце
        layout()->removeWidget(_control_panel_);
        layout()->addWidget(_control_panel_);
//        QVBoxLayout * cur_layout = qobject_cast<QVBoxLayout*>(layout());
//        cur_layout->addWidget(element);
//        int index = cur_layout->indexOf(_control_panel_);

    }
    if(isEditableClass(object_class))
    {
        connectElement(object_class,element);
    }
    if(isContainerClass(object_class))
    {
        QList<QObject*> child_elements(element->children());
        for(QObject* child: child_elements)
        {
            QWidget* child_widget = qobject_cast<QWidget*>(child);
            connectElement(child_widget->objectName(),child_widget);
        }
    }
    return;
}

void InfoWidget::deleteElement(const QString& element_name)
{
    QMap<QString,ObjReinforced>::iterator it = _elements_.find(element_name);
    if(it==_elements_.end())
    {
        return;
    }
    layout()->removeWidget(it.value()._widget_);
    QString className(it.value()._widget_->metaObject()->className());
    if(isEditableClass(className))
    {
        disconnectElement(className,it.value()._widget_);
    }
    delete it.value()._widget_;
    _elements_.erase(it);
    return;
}

void InfoWidget::setReadOnly(bool mode)
{
    //_flags_ = mode? _flags_|IW_ReadOnly : _flags_& ~IW_ReadOnly;
    mode? _flags_|=IW_ReadOnly: _flags_&=~IW_ReadOnly;
    if(mode)
    {
        if(!isHidden() && !isActiveWindow())
        {
            startClosingTimer();
        }
    }
    else
    {
        if(_flags_&IW_OnTimer)
        {
            _hiding_timer_.stop();
            _flags_&=~IW_OnTimer;
        }
    }
    QMap<QString,ObjReinforced>::iterator it = _elements_.begin();
    while(it!=_elements_.end())
    {
        lockElement(it.value()._widget_->metaObject()->className(),it.value()._widget_,!mode);
    }
    return;
}

void InfoWidget::setImmediateResponce(bool mode)
{
    mode? _flags_|=IW_ImmediateResponce: _flags_&=~IW_ImmediateResponce;
    return;
}

bool InfoWidget::setValue(const QString& element_name, const QVariant& value)
{
    QMap<QString,ObjReinforced>::iterator it =_elements_.find(element_name);
    if(it==_elements_.end())
        return false;
    return loadValue(
                it.value()._widget_->metaObject()->className(),
                it.value()._widget_,
                value);
}

QMap<QString,QVariant> InfoWidget::getValues(bool changed_only) const
{
    QMap<QString,QVariant> answer;
    QMap<QString,ObjReinforced>::const_iterator it = _elements_.cbegin();

    while(it!=_elements_.cend())
    {
        QVariant value = getValue(it.value()._widget_);
        if(changed_only)
        {
            if(it->_orig_value_!=value)
            {
                answer.insert(it.key(),value);
            }
        }
        else
        {
            answer.insert(it.key(),value);
        }
        ++it;
    }
    return answer;
}

bool InfoWidget::updElementDefVal(const QString& element_name)
{
    QMap<QString,ObjReinforced>::iterator it = _elements_.find(element_name);
    if(it==_elements_.end())
    {
        return false;
    }
    it.value()._orig_value_ = getValue(it.value()._widget_);
    return true;
}

void InfoWidget::catchElementSignal()
{
    if(_flags_&IW_ImmediateResponce)
    {
        QString cur_object(sender()->objectName());
        QMap<QString,ObjReinforced>::const_iterator it = _elements_.find(cur_object);
        if(it==_elements_.constEnd())
        {
            return;
        }
        QString obj_type(it.value()._widget_->metaObject()->className());
        if(!InfoWidget::isEditableClass(obj_type))
        {
            return;
        }
        QVariant new_val(getValue(it.value()._widget_));
        emit elementValueChanged(cur_object,new_val);
    }
    return;
}

void InfoWidget::saveLocal()
{
    emit saveRequest();
    return;
}

void InfoWidget::closeLocal()
{
    emit closeRequest();
    return;
}

void InfoWidget::catchExternalChange(const QString& element_name, const QVariant& value)
{
    setValue(element_name,value);
    return;
}

void InfoWidget::startClosingTimer(int msec)
{
    _hiding_timer_.start(msec<0?DEF_CLOSING_TIME:msec);
    _flags_|=IW_OnTimer;
    //QTimer::singleShot(msec<0?DEF_CLOSING_TIME:msec,this,&InfoWidget::close);
    return;
}
