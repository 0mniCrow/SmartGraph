#ifndef VERTEXTABLEDELEGATE_H
#define VERTEXTABLEDELEGATE_H
#include <QVBoxLayout>
#include <QStyledItemDelegate>
#include <QFontMetrics>

const int max_icon_size = 64;

class IconDelegate: public QStyledItemDelegate
{
    Q_OBJECT
private:
    QPixmap loadDefImg() const;
public:
    explicit IconDelegate(QObject* tata = nullptr);
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* tata,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    bool editorEvent(QEvent* event,
                     QAbstractItemModel* model,
                     const QStyleOptionViewItem& option,
                     const QModelIndex& index) override;
signals:
    void doubleClickEvent_occured(const QModelIndex& index);
};

struct NameEditor:public QWidget
{
private:
    Q_OBJECT
public:
    QLineEdit* _first_name_;
    QLineEdit* _last_name_;
    QVBoxLayout* _layout_;
    explicit NameEditor(QWidget* tata = nullptr);

};

class NameDelegate:public QStyledItemDelegate
{
    Q_OBJECT
private:
    QString _f_name_;
    QString _l_name_;
public:
    explicit NameDelegate(const QString& first_name = QString(),
                          const QString& last_name = QString(),
                          QObject* tata = nullptr);
    void setFirstName(const QString& first_name);
    void setLastName(const QString& last_name);
    QString getFirstName()const;
    QString getLastName()const;
    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;
    QWidget* createEditor(QWidget* tata,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;
    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override;
};

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
