#ifndef INFOELEMENT_H
#define INFOELEMENT_H
#include "abstractelement.h"
#include <QTextEdit>

class InfoElement:public AbstractElement
{
private:
    QString _value_;
    bool _useHTML_;
    bool _readOnly_;
public:
    enum ElementType{ ET_RichTextElement = AbstractElement::ET_End+3};
    explicit InfoElement(const QString& element_name, char element_type = ET_Default);
    InfoElement(const QString& element_name, const QString& text, bool formatHTML = false, char element_type = ET_Default);
    InfoElement(const QString& element_name, const QVariant& text, bool formatHTML = false, char element_type = ET_Default);
    ~InfoElement();
    QVariant value() const override;
    void setValue(const QVariant& new_val, bool inform_signal = true) override;
    char elementType() const override;
    QWidget* generateWidget() override;
public slots:
    void changeElement(QVariant new_val, bool inform_signal = true) override;
    void setEditable(bool state) override;
};

#endif // INFOELEMENT_H
