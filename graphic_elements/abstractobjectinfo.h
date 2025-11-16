#ifndef ABSTRACTOBJECTINFO_H
#define ABSTRACTOBJECTINFO_H

#define EMPTY_ACTIVE_ELEMENT -1
#include <QList>
#include <initializer_list>
#include <QWidget>
#include "abstractelement.h"

class AbstractObjectInfo:public QObject
{
    Q_OBJECT
private:
    QList<AbstractElement*> _elements_;
    int _active_element_;
    QWidget* _info_widget_;
    AbstractElement* findElement(const QString& el_name) const;
    int findElementNum(const QString& el_name) const;
    bool inSize(int num) const;
protected:
    [[nodiscard]] virtual AbstractElement* createElement(const QString& element_name,
                                           const QVariant& value,
                                           char element_type = AbstractElement::ET_Default) = 0;
    virtual void destroyElement(AbstractElement* element) = 0;
    [[nodiscard]] virtual QWidget* createInfoWindow() = 0;
    virtual void destroyInfoWindow() = 0;
    //virtual bool eventFilter(QObject* obj, QEvent *event) override;
public:
    enum InfoObjectType{IO_Default = 0};
    AbstractObjectInfo();
    AbstractObjectInfo(std::initializer_list<AbstractElement*> list);
    virtual ~AbstractObjectInfo() =0;
    virtual InfoObjectType getType() const {return IO_Default;};
    int size() const;
    bool isExist(const QString& element_name) const;
    QVariant at(int num) const;
    QVariant at(const QString& element_name) const;
    QString nameAt(int num) const;
    AbstractElement* elementAt(int num) const;
    AbstractElement* elementAt(const QString& element_name) const;
    int curElementNum() const;
    QVariant curValue() const;
    QString curElementName() const;
    char curElementType() const;
    AbstractElement* curElement() const;
    AbstractObjectInfo& operator++();
    AbstractObjectInfo& operator--();
    AbstractObjectInfo& operator+(int len);
    AbstractObjectInfo& operator-(int len);
    bool append(const QString& element_name,
                const QVariant& value = QVariant(),
                char element_type = AbstractElement::ET_Default);
    bool appendAt(int num,
                     const QString& element_name,
                     const QVariant& value = QVariant(),
                     char element_type = AbstractElement::ET_Default);
    bool appendAfter(const QString& element_name,
                     const QString& new_element_name,
                     const QVariant& value = QVariant(),
                     char element_type = AbstractElement::ET_Default);
    bool destroyAt(int num);
    bool destroy(const QString& element_name);
    void clear();
    bool setValue(const QString& element_name, const QVariant& new_val);
    bool setValue(int num, const QVariant& new_val);
    QVariant operator[](int num);
    QVariant operator()(const QString& element_name);
    QMap<int,QString> getNames() const;
    bool changeName(const QString& cur_name, const QString& new_name);
    bool changeName(int num, const QString& new_name);
    char widgetType(const QString& element_name) const;
    char widgetType(int num) const;
    bool setWidgetType(const QString& element_name, char element_type);
    bool setWidgetType(int num, char element_type);
    bool swapElements(const QString& first_name, const QString& sec_name);
    bool swapElements(int first_num, int sec_num);
    [[nodiscard]] QWidget * getInfoWidget() const;
public slots:
    virtual void elementValueChanged(const QString& element_name,
                                     const QVariant& value,
                                     bool sendChangeSignal = false) = 0;
signals:
    void elementChange(const QString& element_name, const QVariant& value);
};

#endif // ABSTRACTOBJECTINFO_H
