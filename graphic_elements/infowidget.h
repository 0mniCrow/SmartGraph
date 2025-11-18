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

class InfoWidget : public QWidget
{
    Q_OBJECT
private:
    enum IW_InternalFlags {IW_NoFlags = 0x00, IW_ReadOnly = 0x01, IW_ImmediateResponce = 0x02};
    QTimer _hiding_timer_;
    QMap<QString,QWidget*> _elements_;
    char _flags_;
public:
    enum IW_Orientation{IW_Horisontal=1, IW_Vertical};
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();
    void addElement(QWidget* element, IW_Orientation orientation);
    void setReadOnly(bool mode);
    void setImmediateResponce(bool mode);
private slots:
    void catchElementSignal();
public slots:
    void externalElementChange(const QString& element_name,const QVariant& value);
signals:
    void elementValueChanged(const QString& element_name, const QVariant& value);

};

#endif // INFOWIDGET_H
