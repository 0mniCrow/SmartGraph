#ifndef GVIEW_TOOLTIP_WINDOW_H
#define GVIEW_TOOLTIP_WINDOW_H
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>

class GViewToolTip:public QWidget
{
    Q_OBJECT
private:
    QGroupBox*                          _widget_layer_;
    QList<QPair<QString,QVariant>>      _current_data_;
    QString                             _current_item_type_;
    uint                                _current_item_id_;
    QLabel *                            _info_;
protected:
    virtual void generateWidgetLayer(QStringView item_type);
    virtual void updateValues();
public:
    explicit GViewToolTip(const QString& data, QWidget* tata = nullptr);
    explicit GViewToolTip(QWidget* tata = nullptr);
    void setItemType(QStringView item_type);
    bool setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data);
    const QList<QPair<QString,QVariant>>& getDataList() const;
    QStringView getCurrentItemType();
    uint getCurrentItemID()const noexcept;

public slots:
    void updateFields(const QString& new_val);
};

#endif // GVIEW_TOOLTIP_WINDOW_H
