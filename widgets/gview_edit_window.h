#ifndef GVIEW_EDIT_WINDOW_H
#define GVIEW_EDIT_WINDOW_H
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>

class AbstractGrItem;


class GViewEdit:public QWidget
{
    Q_OBJECT
private:
    QGroupBox*              _widget_layer_;
    QTextEdit*              _text_;
    QString                 _original_text_;
    QString                 _current_item_type_;
    uint                    _current_item_id_;
    QPushButton*            _save_button_;
    QPushButton*            _close_button_;
    QPushButton*            _apply_button_;
    QLayout*                _main_layout_;
    QLayout*                _interface_layout_;
    QList<QPair<QString,QVariant>>  _original_data_;

    void generateMainInterface();
    bool loadDataList(const QList<QPair<QString,QVariant>>& data);
    void updateLayout();
protected:
    virtual void generateWidgetLayer(QStringView item_type);
    virtual void updateValues();
    virtual void saveValues();
    virtual bool checkDataForChanges();
public:
    [[deprecated("Do not attend to new mechanics of edit window")]]explicit GViewEdit(const QString& data, QWidget* tata = nullptr);
    [[deprecated("Do not attend to new mechanics of edit window")]]explicit GViewEdit(AbstractGrItem* first_item = nullptr, QWidget* tata = nullptr);
    explicit GViewEdit(QWidget* tata = nullptr);
    [[deprecated("Do not attend to new mechanics of edit window")]]void setData(const QMap<QString,QString>& fields);
    [[deprecated("Do not attend to new mechanics of edit window")]]void setData(const QString& data);
    [[deprecated("Do not attend to new mechanics of edit window")]]QString getData() const;
    void setItemType(QStringView item_type);
    bool setDataList(uint id, QStringView item_type,const QList<QPair<QString,QVariant>>& data);
    const QList<QPair<QString,QVariant>>& getDataList() const;
    QStringView getCurrentItemType();
    uint getCurrentItemID()const noexcept;
private slots:
    void manualSave();
    void manualClose();
    void manualApply();
public slots:
    [[deprecated("Do not attend to new mechanics of edit window")]]void updateFields(const QString& new_val);
signals:
    [[deprecated("Do not attend to new mechanics of edit window")]]void valueChanged(QString changed_value);
    void itemValueChanged(uint item_id);
};
#endif // GVIEW_EDIT_WINDOW_H
