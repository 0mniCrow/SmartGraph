#ifndef GVIEW_TOOLTIP_WINDOW_H
#define GVIEW_TOOLTIP_WINDOW_H
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTextEdit>

class GViewToolTip:public QWidget
{
    Q_OBJECT
private:
    QGroupBox*                          _data_group_;
    QVBoxLayout*                        _data_layout_;
    QList<QPair<QString,QVariant>>      _current_data_;
    QString                             _current_item_type_;
    uint                                _current_item_id_;
    QLabel *                            _info_;
    void generateMainLayout();
    bool updateGrLayout();
    void loadDataList(const QList<QPair<QString,QVariant>>& data);
protected:
    virtual void generateDataGroup(QStringView item_type);
    virtual void updateValues();
public:
    [[deprecated("Do not attend to new mechanics of tooltip window")]]explicit GViewToolTip(const QString& data, QWidget* tata = nullptr);
    explicit GViewToolTip(QWidget* tata = nullptr);
    bool setItemType(QStringView item_type);
    bool setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data);
    const QList<QPair<QString,QVariant>>& getDataList() const;
    QStringView getCurrentItemType();
    uint getCurrentItemID()const noexcept;

public slots:
    [[deprecated("Do not attend to new mechanics of tooltip window")]]void updateFields(const QString& new_val);
};

#endif // GVIEW_TOOLTIP_WINDOW_H
