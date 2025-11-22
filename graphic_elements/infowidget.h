#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QTimer>
#include <QMap>
#include <QSpinBox>
#include <QSet>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLayout>

struct ObjReinforced
{
    QWidget* _widget_;
    QVariant _orig_value_;
    ObjReinforced(QWidget* widget,
                  const QVariant& orig_value):
        _widget_(widget),_orig_value_(orig_value){}
};

class InfoWidget : public QWidget
{
    Q_OBJECT
private:
    enum IW_InternalFlags {IW_NoFlags = 0x00, IW_ReadOnly = 0x01, IW_ImmediateResponce = 0x02};
    QTimer _hiding_timer_;
    QMap<QString,ObjReinforced> _elements_;
    char _flags_;
    void connectElement(const QString& type, QWidget* element);
    bool loadValue(const QString& type, QWidget* element, const QVariant& value);
    QVariant getValue(QWidget* widget) const;
    static bool isContainerClass(const QString& class_name);
    static bool isEditableClass(const QString& class_name);
    static bool isAllowedClass(const QString& class_name);
public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();
    void addElement(QWidget* element);
    void setReadOnly(bool mode);
    void setImmediateResponce(bool mode);
    bool setValue(const QString& element_name, const QVariant& value);
    QMap<QString,QVariant> getValues(bool changed_only) const;
private slots:
    void catchElementSignal();
//    void save();
    void close();
public slots:
    void externalElementChange(const QString& element_name,const QVariant& value);
signals:
    void elementValueChanged(const QString& element_name, const QVariant& value);
    void hasChanges();

};

#endif // INFOWIDGET_H
