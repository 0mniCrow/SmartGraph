#include "gview_tableVertexTableDelegate.h"
#include "qapplication.h"
#include "qpainter.h"
#include <QLineEdit>

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
    QVariant data = index.data();
    ///!TODO get image form index
    painter->fillRect(option.rect,option.state & QStyle::State_Selected?
                          option.palette.highlight():
                          option.palette.base());
    if(image.isNull())
    {
        image = loadDefImg();
    }
    QSize size = image.size();
    if(size.height()!=pref_icon_size || size.width()!=pref_icon_size)
    {
        image = image.scaled(pref_icon_size,pref_icon_size);
    }
    QPoint center = option.rect.center();
    painter->drawPixmap(center,image);
    painter->restore();
    return;
}

//QWidget* IconDelegate::createEditor([[maybe_unused]]QWidget* tata,
//                           [[maybe_unused]]const QStyleOptionViewItem& option,
//                           [[maybe_unused]]const QModelIndex& index) const
//{
//    Q_UNUSED(tata)
//    Q_UNUSED(option)
//    Q_UNUSED(index)
//    return nullptr;
//}

bool IconDelegate::editorEvent(QEvent* event,
                      QAbstractItemModel* model,
                      const QStyleOptionViewItem& option,
                      const QModelIndex& index)
{
    Q_UNUSED(model)
    Q_UNUSED(option)
    if(event->type() == QEvent::MouseButtonDblClick)
    {
        emit doubleClickEvent_occured(index);
    }
    return false;
}

NameEditor::NameEditor(QWidget* tata):QWidget(tata)
{
    _layout_ = new QVBoxLayout(this);
    _first_name_ = new QLineEdit(this);
    _last_name_ = new QLineEdit(this);
    _layout_->addWidget(_first_name_);
    _layout_->addWidget(_last_name_);
    return;
}

NameDelegate::NameDelegate(QObject* tata):
    QStyledItemDelegate(tata)
{
    return;
}

void NameDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& option,
           const QModelIndex& index) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing,true);
    QVariant data = index.data(); ///!TODO get values from index' data
    QString f_name,l_name;
    QFont cur_font("Times",12, QFont::Normal);
    QFontMetrics font_metrics(cur_font);
    int max_str_p_size = std::max(font_metrics.horizontalAdvance(f_name),font_metrics.horizontalAdvance(l_name));
    QRect text_rectangle1(0,0,max_str_p_size,pref_icon_size/2);
    QRect text_rectangle2(0,pref_icon_size/2,max_str_p_size,pref_icon_size/2);
    painter->setFont(cur_font);
    painter->drawText(text_rectangle1,Qt::AlignCenter|Qt::AlignVCenter,f_name);
    painter->drawText(text_rectangle2,Qt::AlignCenter|Qt::AlignVCenter,l_name);

    /*
        QString str("Short");
        QString str1("Long Long String 2 ");
        QFont cur_font("Times",12, QFont::Normal);
        QFontMetrics font_metrics(cur_font);
        int max_str_p_size = std::max(font_metrics.horizontalAdvance(str),font_metrics.horizontalAdvance(str1));
        const int ellipce_size = 64;
        const int space = 10;
        QRect rectangle(0,0,ellipce_size+space*2+max_str_p_size,ellipce_size);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setBrush(QBrush(Qt::darkRed));
        painter.setPen(QPen(Qt::black,2));
        painter.drawRect(rectangle);
        painter.drawEllipse(0,0,ellipce_size,ellipce_size);
        QRect text_rectangle(ellipce_size+space,0,max_str_p_size,ellipce_size/2);
        QRect text_rectangle2(ellipce_size+space,ellipce_size/2,max_str_p_size,ellipce_size/2);
        painter.setFont(cur_font);
        painter.drawText(text_rectangle,Qt::AlignCenter,str);
        painter.drawText(text_rectangle2,Qt::AlignCenter,str1);
*/


    painter->restore();
}

