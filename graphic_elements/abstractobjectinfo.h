#ifndef ABSTRACTOBJECTINFO_H
#define ABSTRACTOBJECTINFO_H
#include <QList>
#include <initializer_list>
#include <iterator>
#include "abstractelement.h"

class AbstractObjectInfo
{
private:
    QList<AbstractElement*> _elements_;
    AbstractElement* findElement(const QString& el_name) const;
    long _elem_memory_;
protected:
    virtual AbstractElement* createElement(const QString& element_name,
                                           const QVariant& value,
                                           char element_type = AbstractElement::ET_Default) = 0;
    virtual void destroyElement(AbstractElement* element) = 0;
public:
    AbstractObjectInfo();
    AbstractObjectInfo(std::initializer_list<AbstractElement*> list);
    ~AbstractObjectInfo();
    int size();
    void setSize(int size);
    bool isExist(const QString& element_name) const;
    QVariant at(int num) const;
    QVariant at(const QString& element_name) const;
    QString nameAt(int num) const;
    AbstractElement* elementAt(int num) const;
    AbstractElement* elementAt(const QString& element_name) const;
    bool append(const QString& elenemt_name,
                const QVariant& value = QVariant(),
                char element_type = AbstractElement::ET_Default);
    bool destroyAt(int num);
    bool destroy(const QString& element_name);
    void clear();
};

#endif // ABSTRACTOBJECTINFO_H
