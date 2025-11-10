#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include "abstractelement.h"

class ImageElement:public AbstractElement
{
private:
    QPixmap _value_;
public:
    enum PicType{PT_Default = 0, PT_RoundIcon = 1,
                 PT_RoundPic = 2, PT_SquareIcon = 3,
                PT_SquarePic = 4, PT_TriangleIcon = 5,
                PT_TrianglePic = 6};
    enum ElementType{ ET_PictureElement = AbstractElement::ET_End+2};
    explicit ImageElement(const QString& element_name, char element_type = ET_Default);
    ImageElement(const QString& element_name, const QPixmap& value, char element_type = ET_Default);
    ImageElement(const QString& element_name, const QVariant& value, char element_type = ET_Default);
    ~ImageElement();
    QVariant value() const override;
    void setValue(const QVariant& new_val, bool inform_signal = true) override;
    char elementType() const override;
    QWidget* generateWidget() override;
    QWidget* generatePic(char picture_type);
    void setImage(const QImage& image, const QRect& rect= QRect());
    void setImage(const QPixmap& pixmap, qreal radius, const QPointF& central_point= QPointF());
public slots:
    void changeElement(QVariant new_val, bool inform_signal = true) override;
    void setEditable(bool state) override;
};

#endif // IMAGEELEMENT_H
