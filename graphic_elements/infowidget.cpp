#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget{parent}
{

}

InfoWidget::~InfoWidget()
{

}

void InfoWidget::closeEvent(QCloseEvent*  cl_event)
{
    if((_flags_&IW_ReadOnly)||
            (_flags_&IW_OnTimer)||
            !hasChanged())
    {
        cl_event->accept();
        return;
    }
    QMessageBox mbx;
    mbx.setText("Вы ўпэўнены што жадаеце закрыць акно?");
    mbx.setInformativeText("Пэўныя зьмяненьні не захаваны.");
    mbx.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
    mbx.setDefaultButton(QMessageBox::Yes);
    mbx.setIcon(QMessageBox::Warning);
    int vynik =mbx.exec();
    switch(vynik)
    {
    case QMessageBox::Yes:
    {
        cl_event->accept();
    }
        break;
    case QMessageBox::No:
    {
        cl_event->ignore();
    }
        break;
    }
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
    while(true)
    {
        if(type == "QSpinBox")
        {
                QSpinBox* obj = qobject_cast<QSpinBox*>(element);
                connect(obj,&QSpinBox::valueChanged,this,&InfoWidget::catchElementSignal);
                break;
        }
        if(type == "QTextEdit")
        {
                QTextEdit* obj = qobject_cast<QTextEdit*>(element);
                connect(obj,&QTextEdit::textChanged,this,&InfoWidget::catchElementSignal);
                break;
        }
        if(type == "QLineEdit")
        {
                QLineEdit* obj = qobject_cast<QLineEdit*>(element);
                connect(obj,&QLineEdit::textEdited,this,&InfoWidget::catchElementSignal);
                break;
        }
        if(type == "QComboBox")
        {
                QComboBox* obj = qobject_cast<QComboBox*>(element);
                connect(obj,&QComboBox::currentIndexChanged,this,&InfoWidget::catchElementSignal);
                break;
        }
        return;
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
    while(true)
    {
        if(type == "QSpinBox")
        {
                QSpinBox* obj = qobject_cast<QSpinBox*>(element);
                if(value.canConvert<int>())
                {
                    obj->setValue(value.toInt());
                    break;
                }
        }
        if(type == "QTextEdit")
        {
                QTextEdit* obj = qobject_cast<QTextEdit*>(element);
                if(value.canConvert<QString>())
                {
                    obj->setText(value.toString());
                    break;
                }
        }
        if(type == "QLineEdit")
        {
                QLineEdit* obj = qobject_cast<QLineEdit*>(element);
                if(value.canConvert<QString>())
                {
                    obj->setText(value.toString());
                    break;
                }
        }
        if(type == "QComboBox")
        {
                QComboBox* obj = qobject_cast<QComboBox*>(element);
                if(value.canConvert<int>())
                {
                    obj->setCurrentIndex(value.toInt());
                    break;
                }
        }
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
        layout()->addWidget(element);
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

void InfoWidget::setReadOnly(bool mode)
{
    //_flags_ = mode? _flags_|IW_ReadOnly : _flags_& ~IW_ReadOnly;
    mode? _flags_|=IW_ReadOnly: _flags_&=~IW_ReadOnly;
}

void InfoWidget::setImmediateResponce(bool mode)
{
    mode? _flags_|=IW_ImmediateResponce: _flags_&=~IW_ImmediateResponce;
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

//void InfoWidget::save()
//{
//    if(_flags_&IW_ImmediateResponce)
//    {
//        QMap<QString,ObjReinforced>::const_iterator it = _elements_.cbegin();
//        while(it!=_elements_.cend())
//        {
//            QVariant value = getValue(it.value()._widget_);

//            ++it;
//        }
//    }
//}

void InfoWidget::externalElementChange(const QString& element_name,const QVariant& value)
{

}

void InfoWidget::elementValueChanged(const QString& element_name, const QVariant& value)
{

}

void InfoWidget::catchElementSignal()
{
    QString cur_object(sender()->objectName());
}
