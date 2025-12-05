#ifndef NODEOBJECTINFO_H
#define NODEOBJECTINFO_H

#include "infowidget.h"
#include "abstractobjectinfo.h"
#include "nameelement.h"
#include "imageelement.h"
#include "infoelement.h"
#include "dateelement.h"

class NodeObjectInfo:public AbstractObjectInfo
{
private:
    InfoWidget* _widget_;

    [[nodiscard]] AbstractElement* createElementLoc(const QString& element_name,
                                   const QVariant& value,
                                   char element_type = AbstractElement::ET_Default);
    void destroyElementLoc(AbstractElement * element);
    void createInfoWidgetLoc();
    void destroyInfoWindowLoc();
    void copyObject(const NodeObjectInfo& other);
    void deepCopyObject(NodeObjectInfo&& other);

    template<typename T>
    static T transElem(AbstractElement* element)
    {
        return dynamic_cast<T>(element);
    }
protected:
    [[nodiscard]] AbstractElement* createElement(const QString& element_name,
                                           const QVariant& value,
                                           char element_type = AbstractElement::ET_Default) override;
    void destroyElement(AbstractElement* element) override;
    [[nodiscard]] QWidget* createInfoWindow() override;
    void destroyInfoWindow() override;
public:
    NodeObjectInfo();
    NodeObjectInfo(const NodeObjectInfo& other);
    NodeObjectInfo(NodeObjectInfo&& other);
    ~NodeObjectInfo();
    [[nodiscard]] virtual QWidget * getInfoWidget() override;
    QList<QVariant> shortInfo() const override;
    QList<QVariant> fullInfo() const override;
    void setReadOnly(bool mode);
    void resetWidget();
    void clearObject();
    NodeObjectInfo& operator=(const NodeObjectInfo&other);
    NodeObjectInfo& operator=(NodeObjectInfo&&other);
    static NodeObjectInfo generateNodeInfo(const QMap<QString,std::pair<QVariant,char>>& elements);
public slots:
    void widgetValueChanged(const QString& element_name,
                                     const QVariant& value) override;
    void externalValueChanged(const QString& element_name,
                                      const QVariant& value) override;
    void saveRequest() override;
    void closeRequest() override;
};

#endif // NODEOBJECTINFO_H
