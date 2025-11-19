#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QTimer>
#include <QMap>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QLayout>

class InfoWidget : public QWidget
{
    Q_OBJECT
private:
    enum IW_InternalFlags {IW_NoFlags = 0x00, IW_ReadOnly = 0x01, IW_ImmediateResponce = 0x02};
    QTimer _hiding_timer_;
    QMap<QString,QWidget*> _elements_;
    char _flags_;
    void connectElement(const QString& type, QWidget* element);
    bool loadValue(const QString& type, QWidget* element, const QVariant& value);
    static bool isContainerClass(const QString& class_name);
    static bool isEditableClass(const QString& class_name);
    static bool isAllowedClass(const QString& class_name);
public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();
    void addElement(QWidget* element);
    void save();
    void setReadOnly(bool mode);
    void setImmediateResponce(bool mode);
    bool setValue(const QString& element_name, const QVariant& value);
private slots:
    void catchElementSignal();
public slots:
    void externalElementChange(const QString& element_name,const QVariant& value);
signals:
    void elementValueChanged(const QString& element_name, const QVariant& value);

};

#endif // INFOWIDGET_H
