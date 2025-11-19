#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget{parent}
{

}

InfoWidget::~InfoWidget()
{

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

void InfoWidget::addElement(QWidget* element)
{
    QString object_class(element->metaObject()->className());

    if(isAllowedClass(object_class))
    {
        _elements_.insert(element->objectName(),element);
        layout()->addWidget(element);
    }
    if((_flags_&IW_ImmediateResponce)&&isEditableClass(object_class))
    {
        connectElement(object_class,element);
    }
    if((_flags_&IW_ImmediateResponce)&&isContainerClass(object_class))
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
    QMap<QString,QWidget*>::iterator it =_elements_.find(element_name);
    if(it==_elements_.end())
        return false;
    return loadValue(
                it.value()->metaObject()->className(),
                it.value(),
                value);

}

void InfoWidget::save()
{

}

void InfoWidget::externalElementChange(const QString& element_name,const QVariant& value)
{

}

void InfoWidget::elementValueChanged(const QString& element_name, const QVariant& value)
{

}

void InfoWidget::catchElementSignal()
{

}
