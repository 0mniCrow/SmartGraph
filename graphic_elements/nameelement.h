#ifndef NAMEELEMENT_H
#define NAMEELEMENT_H
#define NAME_ELEMENT_MAX_LEN 255
#include "abstractelement.h"
#include <QLineEdit>
#include <QLabel>

class NameElement:public AbstractElement
{
private:
    QString _value_;
    bool _is_editable_;
public:
    enum ElementType{ ET_NameElement = AbstractElement::ET_End+1};
    explicit NameElement(const QString& element_name, char element_type = ET_Default);
    NameElement(const QString& element_name, const QString& value, char element_type = ET_Default);
    NameElement(const QString& element_name, const QVariant& value, char element_type = ET_Default);
    ~NameElement() override;
    QVariant value() const override;
    void setValue(const QVariant& new_val, bool inform_signal = true) override;
    char elementType() const override;
    [[nodiscard]] QWidget* generateWidget() override;
    QString internalDataType() const override;
public slots:
    void changeElement(QVariant new_val, bool inform_signal = true) override;
    void setEditable(bool state) override;
};

#endif // NAMEELEMENT_H
