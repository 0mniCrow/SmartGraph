#ifndef IMAGEELEMENT_H
#define IMAGEELEMENT_H
#define IMAGE_ICON_SIZE 64
#include <QPixmap>
#include <QImage>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include "abstractelement.h"

class ImageLabel:public QLabel
{
private:
    QPixmap _cur_pixmap_;
public:
    ImageLabel(QWidget* tata = nullptr):QLabel(tata){};
    void setPixmap(const QPixmap& val){_cur_pixmap_ = val; QLabel::setPixmap(_cur_pixmap_);};
    QPixmap getPixmap() const {return _cur_pixmap_;};
protected:
    void mousePressEvent(QMouseEvent * m_event) override;
signals:
    void doubleClickEvent();
};

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
    [[nodiscard]] QWidget* generateWidget() override;
    QString internalDataType() const override;
    QPixmap generatePixmap(char picture_type, int icon_size = IMAGE_ICON_SIZE);
    [[nodiscard]] QWidget* generateSpecWidget(char picture_type, int icon_size = IMAGE_ICON_SIZE);
    void setImage(const QImage& image, const QRect& rect= QRect(), bool inform_signal = true);
    void setImage(const QPixmap& pixmap, int side_size, bool keep_aspect_ratio = true, bool inform_signal = true);
public slots:
    void changeElement(QVariant new_val, bool inform_signal = true) override;
    void setEditable(bool state) override;
};

#endif // IMAGEELEMENT_H
