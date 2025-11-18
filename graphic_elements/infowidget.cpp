#include "infowidget.h"

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget{parent}
{

}

InfoWidget::~InfoWidget()
{

}

void InfoWidget::addElement(QWidget* element, IW_Orientation orientation)
{
    QString object_class(element->metaObject()->className());
    while(true)
    {
        if(object_class == "QSpinBox")
        {
            _elements_.insert(element->objectName(),element);

            if(_flags_&IW_ImmediateResponce)
            {
                QSpinBox* obj = qobject_cast<QSpinBox*>(element);
                connect(obj,&QSpinBox::valueChanged,this,&InfoWidget::catchElementSignal);
            }
            break;
        }
        if(object_class == "QTextEdit")
        {
            _elements_.insert(element->objectName(),element);
            if(_flags_&IW_ImmediateResponce)
            {
                QTextEdit* obj = qobject_cast<QTextEdit*>(element);
                connect(obj,&QTextEdit::textChanged,this,&InfoWidget::catchElementSignal);
            }
            break;
        }
        if(object_class == "QLineEdit")
        {
            _elements_.insert(element->objectName(),element);
            if(_flags_&IW_ImmediateResponce)
            {
                QLineEdit* obj = qobject_cast<QLineEdit*>(element);
                connect(obj,&QLineEdit::textEdited,this,&InfoWidget::catchElementSignal);
            }
            break;
        }
        if(object_class == "QComboBox")
        {
            _elements_.insert(element->objectName(),element);
            {
                QComboBox* obj = qobject_cast<QComboBox*>(element);
                connect(obj,&QComboBox::currentIndexChanged,this,&InfoWidget::catchElementSignal);
            }
            break;
        }
        if(object_class == "QLabel")
        {
            _elements_.insert(element->objectName(),element);
            break;
        }
        return;
    }
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
