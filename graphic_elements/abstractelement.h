#ifndef ABSTRACTELEMENT_H
#define ABSTRACTELEMENT_H
#include <QVariant>
#include <QString>
#include <QWidget>
//#include <QObject>

class AbstractElement:public QObject
{
    Q_OBJECT
public:
    enum ElementType{ET_Error = -1,
                     ET_Default = 0,
                     ET_LineEdit = 1,
                     ET_TextEdit = 2,
                     ET_SpinBox = 3,
                     ET_ComboBox = 4,
                     ET_Slider = 5,
                     ET_End = 6};
    explicit AbstractElement(const QString& element_name, char element_type = ET_Default);
    virtual ~AbstractElement();
    virtual QVariant value() const = 0;
    virtual void setValue(const QVariant& new_val, bool inform_signal = true) = 0;
    QString elementName() const;
    void setElementName(const QString& new_name);
    virtual char elementType() const;
    void setElementType(char type);
    [[nodiscard]] virtual QWidget* generateWidget() = 0;  //Метад разлічаны на тое, што
                                                            //іншы элемент возьме кантроль над ім (выдаліць)
signals:
    //void valueChanged(const QVariant& new_val);
    void elementChanged(const QString& element_name, QVariant new_val);
    void elementDestroyed(QString element_name);
public slots:
    virtual void changeElement(QVariant new_val, bool inform_signal = true) = 0;
    //virtual void saveValue() = 0;
    virtual void setEditable(bool state) = 0;
protected:
    QString _element_name_;
    char _element_type_;
};

#endif // ABSTRACTELEMENT_H
