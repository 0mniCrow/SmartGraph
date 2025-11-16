#ifndef NODEOBJECTINFO_H
#define NODEOBJECTINFO_H

#include "abstractobjectinfo.h"
#include "nameelement.h"
#include "imageelement.h"
#include "infoelement.h"
#include "dateelement.h"

class NodeObjectInfo:public AbstractObjectInfo
{
public:
    NodeObjectInfo();
protected:
    [[nodiscard]] AbstractElement* createElement(const QString& element_name,
                                           const QVariant& value,
                                           char element_type = AbstractElement::ET_Default) override;
    void destroyElement(AbstractElement* element) override;
    [[nodiscard]] QWidget* createInfoWindow() override;
    void destroyInfoWindow() override;

};

#endif // NODEOBJECTINFO_H
