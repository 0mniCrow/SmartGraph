#ifndef VERTEXTABLEDELEGATE_H
#define VERTEXTABLEDELEGATE_H
#include <QStyledItemDelegate>
#include <QFontMetrics>

/*
class TableInfo
{
private:
    QPixmap _image_;
    QString _info_;
    QString _additional_info_;
public:
    enum class Mode{ReadMode,EditMode};
    TableInfo(const QPixmap& image, const QString& info,
                       const QString& additional_info = QString());
    bool setImage(const QPixmap& new_image);
    bool setInfo(const QString& new_info);
    bool setAdditionalInfo(const QString& new_add_info);
    QPixmap getImage() const;
    QString getInfo() const;
    QString getAdditionalInfo() const;
    QSize sizeHint() const;
    void paint(QPainter* painter, const QRect& rect,
               const QPalette& palette, Mode mode) const;

};

Q_DECLARE_METATYPE(TableInfo)

class VertexTableDelegate:public QStyledItemDelegate
{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    VertexTableDelegate();
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;
    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;

};
*/
#endif // VERTEXTABLEDELEGATE_H
