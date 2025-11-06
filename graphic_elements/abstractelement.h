#ifndef ABSTRACTELEMENT_H
#define ABSTRACTELEMENT_H
#include <QVariant>
#include <QString>

class AbstractElement
{
private:
    QString _element_name_;
    char _element_type_;
public:
    enum ElementType{ET_Default = 0,
                     ET_LineEdit = 1,
                     ET_TextEdit = 2,
                     ET_SpinBox = 3,
                     ET_ComboBox = 4,
                     ET_Slider = 5,
                     ET_End = 6};
    explicit AbstractElement(const QString& element_name, char element_type = ET_Default);
    virtual ~AbstractElement();
    virtual QVariant value() const = 0;
    virtual QVariant& rvalue() = 0;
    virtual void setValue(const QVariant& new_val, bool inform_signal = true) = 0;
    QString elementName() const;
    void setElementType(const QString& new_name);
    char elementType() const;
    void setElementType(char type);
signals:
    void valueChanged(const QVariant& new_val);
    void elementChanged(const QString& element_name, const QVariant& new_val);
    void elementDestroyed(QString element_name);
public slots:
    virtual void changeElement(QVariant new_val, bool inform_signal = true) = 0;

};

#endif // ABSTRACTELEMENT_H
