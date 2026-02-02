#include "gview_tableVertexTableDelegate.h"
#include "qapplication.h"
#include "qpainter.h"

IconDelegate::IconDelegate(QObject* tata):
    QStyledItemDelegate(tata)
{
    return;
}

QPixmap IconDelegate::loadDefImg() const
{
    ///!TODO default image
    return QPixmap();
}

void IconDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& option,
           const QModelIndex& index) const
{
    painter->save();
    QPixmap image;
    ///!TODO get image form index
    painter->fillRect(option.rect,option.state & QStyle::State_Selected?
                          option.palette.highlight():
                          option.palette.base());
    if(image.isNull())
    {
        image = loadDefImg();
    }
    QSize size = image.size();


}

QWidget* IconDelegate::createEditor(QWidget* tata,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{

}

bool IconDelegate::editorEvent(QEvent* event,
                      QAbstractItemModel* model,
                      const QStyleOptionViewItem& option,
                      const QModelIndex& index)
{

}



/*
TableInfo::TableInfo(const QPixmap& image, const QString& info,
                   const QString& additional_info)
{
    _image_ = image;
    _info_ = info;
    _additional_info_ = additional_info;
    return;
}

bool TableInfo::setImage(const QPixmap& new_image)
{
    if(new_image.isNull())
        return false;
    _image_ = new_image;
    return true;
}
bool TableInfo::setInfo(const QString& new_info)
{
    if(new_info.isEmpty())
    {
        return false;
    }
    _info_ = new_info;
    return true;
}

bool TableInfo::setAdditionalInfo(const QString& new_add_info)
{
    _additional_info_ = new_add_info;
    return true;
}

QPixmap TableInfo::getImage() const
{
    return _image_;
}

QString TableInfo::getInfo() const
{
    return _info_;
}

QString TableInfo::getAdditionalInfo() const
{
    return _additional_info_;
}
QSize TableInfo::sizeHint() const
{
    QFont local_font(QApplication::font());
    QFontMetrics font_metrics(local_font);
    QSize local_size(_image_.width()+font_metrics.horizontalAdvance(_info_),_image_.height());
    return local_size;
}

void TableInfo::paint(QPainter* painter, const QRect& rect,
           const QPalette& palette, Mode mode) const
{

}


VertexTableDelegate::VertexTableDelegate()
{

}

void VertexTableDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& option,
           const QModelIndex& index) const
{

}
*/
